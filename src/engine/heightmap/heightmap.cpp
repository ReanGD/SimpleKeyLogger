#include "engine/heightmap/heightmap.h"

#include <noise.h>
#include <fmt/format.h>
#include <noise/noiseutils.h>

#include "engine/gui/widgets.h"
#include "engine/material/image_loader.h"
#include "engine/material/texture_manager.h"

static const char* QualityItems[] = {"Fast", "Std", "Best"};

static noise::NoiseQuality ToNoiseType(const Heightmap::Quality value) {
    switch (value) {
    case Heightmap::Quality::Fast: return noise::NoiseQuality::QUALITY_FAST;
    case Heightmap::Quality::Std: return noise::NoiseQuality::QUALITY_STD;
    case Heightmap::Quality::Best: return noise::NoiseQuality::QUALITY_BEST;
    default: return noise::NoiseQuality::QUALITY_STD;
    }
}

bool Heightmap::Create(std::string& error) noexcept {
    module::Perlin perlinModule;
    perlinModule.SetFrequency(m_frequency);
    perlinModule.SetLacunarity(m_lacunarity);
    perlinModule.SetNoiseQuality(ToNoiseType(m_noiseQuality));
    perlinModule.SetOctaveCount(m_octaveCount);
    perlinModule.SetPersistence(m_persistence);

    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule(perlinModule);
    heightMapBuilder.SetDestNoiseMap(heightMap);
    heightMapBuilder.SetDestSize(256, 256);
    heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
    heightMapBuilder.Build();

    utils::RendererImage renderer;
    utils::Image image;
    renderer.SetSourceNoiseMap(heightMap);
    renderer.SetDestImage(image);

    renderer.ClearGradient();
    renderer.AddGradientPoint(-1.0000, utils::Color(  0,   0, 128, 255)); // deeps
    renderer.AddGradientPoint(-0.2500, utils::Color(  0,   0, 255, 255)); // shallow
    renderer.AddGradientPoint( 0.0000, utils::Color(  0, 128, 255, 255)); // shore
    renderer.AddGradientPoint( 0.0625, utils::Color(240, 240,  64, 255)); // sand
    renderer.AddGradientPoint( 0.1250, utils::Color( 32, 160,   0, 255)); // grass
    renderer.AddGradientPoint( 0.3750, utils::Color(224, 224,   0, 255)); // dirt
    renderer.AddGradientPoint( 0.7500, utils::Color(128, 128, 128, 255)); // rock
    renderer.AddGradientPoint( 1.0000, utils::Color(255, 255, 255, 255)); // snow
    renderer.EnableLight();
    renderer.SetLightContrast(3.0);
    renderer.SetLightBrightness(2.0);

    renderer.Render();

    size_t cntPixel = image.GetMemUsed();
    utils::Color* colors = image.GetSlabPtr();
    size_t outBytePerPixel = 4;
    uint8* texData = new uint8[cntPixel * outBytePerPixel];
    for (size_t i=0 ; i!=cntPixel; ++i) {
        utils::Color color = colors[i];
        texData[i * outBytePerPixel + 0] = color.red;
        texData[i * outBytePerPixel + 1] = color.green;
        texData[i * outBytePerPixel + 2] = color.blue;
        texData[i * outBytePerPixel + 3] = color.alpha;
    }

    ImageHeader header(256, 256, PixelFormat::R8G8B8A8);
    m_previewTex = TextureManager::Get().Create(Image(header, 1, texData), error);
    delete []texData;
    if (!m_previewTex) {
        return false;
    }

    return true;
}

std::shared_ptr<PhysicalNode> Heightmap::Load(const std::filesystem::path& path, std::string& error) noexcept {
    std::filesystem::path fullPath;
    if (!TextureManager::Get().GetFullPath(path, fullPath, error)) {
        return nullptr;
    }

    Image image;
    if(!image_loader::Load(fullPath.c_str(), image, true, error)) {
        return nullptr;
    }

    if (image.header.format != PixelFormat::R8G8B8) {
        error = fmt::format("wrong image ({}) format for heightmap, expected: {}, actual: '{}'",
            fullPath.c_str(), ToStr(PixelFormat::R8G8B8), ToStr(image.header.format));
        image_loader::Free(image);
        return nullptr;
    }

    if (image.header.width != image.header.height) {
        error = fmt::format("wrong image ({}) size for heightmap, expected: width == height, actual: width = {}, height = {}",
            fullPath.c_str(), image.header.width, image.header.height);
        image_loader::Free(image);
        return nullptr;
    }

    uint8_t* data = reinterpret_cast<uint8_t*>(image.data);
    uint32_t gridSize = image.header.width;
    float heightScale = 0.1f;
    size_t bytesPerElement = sizeof(uint8_t);
    float minHeight, maxHeight;

    uint8_t* rawHeightfieldData = new uint8_t[static_cast<size_t>(gridSize) * static_cast<size_t>(gridSize) * bytesPerElement];
    uint8_t* p = rawHeightfieldData;
    for (uint32_t j=0; j!=gridSize; ++j) {
        for (uint32_t i=0; i!=gridSize; ++i) {
            uint8_t rawZ = data[i * 3 + gridSize * j * 3];
            float z = static_cast<float>(rawZ) * heightScale;
            *p = rawZ;

            if (!i && !j) {
                minHeight = z;
                maxHeight = z;
            } else {
                if (z < minHeight) {
                    minHeight = z;
                }
                if (z > maxHeight) {
                    maxHeight = z;
                }
            }

            p += bytesPerElement;
        }
    }

    image_loader::Free(image);

    // TODO: remove rawHeightfieldData
    return std::make_shared<PhysicalTerrain>(gridSize, rawHeightfieldData, heightScale, minHeight, maxHeight, 0.5f * (minHeight + maxHeight));
}

void Heightmap::DrawSettings() {
    bool changed = false;

    changed |= gui::Combo("Quality", m_noiseQuality, QualityItems, Quality::Count);
    changed |= gui::InputScalar("Frequency", m_frequency, gui::Step(0.1, 1.0), "%.1f");
    changed |= gui::InputScalar("Lacunarity", m_lacunarity, gui::Step(0.01, 0.1), gui::Range(1.5, 3.5), "%.2f");
    changed |= gui::InputScalar("Octave count", m_octaveCount,
        gui::Step(uint8_t(1), uint8_t(2)),
        gui::Range(uint8_t(1), static_cast<uint8_t>(noise::module::PERLIN_MAX_OCTAVE)));
    changed |= gui::InputScalar("Persistence", m_persistence, gui::Step(0.01, 0.1), gui::Range(0.0, 1.0), "%.2f");

    if (changed) {
        std::string error;
        if (!Create(error)) {
            // spdlog::error(error);
        }
    }
}
