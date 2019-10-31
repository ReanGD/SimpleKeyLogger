#include "engine/heightmap/heightmap.h"

#include <noise.h>
#include <fmt/format.h>

#include "engine/gui/widgets.h"
#include "engine/material/texture_manager.h"
#include "middleware/node_editor/noiseutils.h"

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
    auto image = Generate();
    m_previewTex = TextureManager::Get().Create(image, error);
    uint8* texData = reinterpret_cast<uint8*>(image.data);
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
    if(!image.Load(fullPath.c_str(), true, error)) {
        return nullptr;
    }

    auto header = image.view.header;
    if (header.format != PixelFormat::R8G8B8) {
        error = fmt::format("wrong image ({}) format for heightmap, expected: {}, actual: '{}'",
            fullPath.c_str(), ToStr(PixelFormat::R8G8B8), ToStr(header.format));
        return nullptr;
    }

    if (header.width != header.height) {
        error = fmt::format("wrong image ({}) size for heightmap, expected: width == height, actual: width = {}, height = {}",
            fullPath.c_str(), header.width, header.height);
        return nullptr;
    }

    uint8_t* data = reinterpret_cast<uint8_t*>(image.view.data);
    uint32_t gridSize = header.width;
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

    // TODO: remove rawHeightfieldData
    return std::make_shared<PhysicalTerrain>(gridSize, rawHeightfieldData, heightScale, minHeight, maxHeight, 0.5f * (minHeight + maxHeight));
}

ImageView Heightmap::Generate() const noexcept {
    module::Perlin perlinModule;
    perlinModule.SetFrequency(m_frequency);
    perlinModule.SetLacunarity(m_lacunarity);
    perlinModule.SetNoiseQuality(ToNoiseType(m_noiseQuality));
    perlinModule.SetOctaveCount(m_octaveCount);
    perlinModule.SetPersistence(m_persistence);
    perlinModule.SetSeed(m_seed);

    module::RidgedMulti mountainTerrain;

    module::Billow baseFlatTerrain;
    baseFlatTerrain.SetFrequency(2.0);

    module::ScaleBias flatTerrain;
    flatTerrain.SetScale(0.125);
    flatTerrain.SetBias(-0.75);
    flatTerrain.SetSourceModule(0, baseFlatTerrain);

    module::Select finalTerrain;
    finalTerrain.SetSourceModule(0, flatTerrain);
    finalTerrain.SetSourceModule(1, mountainTerrain);
    finalTerrain.SetControlModule(perlinModule);
    finalTerrain.SetBounds(0.0, 1000.0);
    finalTerrain.SetEdgeFalloff(0.125);

    utils::PlaneShape planeShape;
    planeShape.SetSourceModule(finalTerrain);

    utils::Image image;
    utils::RendererImage renderer;
    renderer.SetSourceModule(planeShape);
    renderer.SetDestImage(image);
    renderer.SetDestSize(256, 256);
    renderer.SetBounds(2.0, 6.0, 1.0, 5.0);
    renderer.Render();

    renderer.ClearGradient();
    renderer.AddGradientPoint(-1.0000, math::Color(  0,   0, 128)); // deeps
    renderer.AddGradientPoint(-0.2500, math::Color(  0,   0, 255)); // shallow
    renderer.AddGradientPoint( 0.0000, math::Color(  0, 128, 255)); // shore
    renderer.AddGradientPoint( 0.0625, math::Color(240, 240,  64)); // sand
    renderer.AddGradientPoint( 0.1250, math::Color( 32, 160,   0)); // grass
    renderer.AddGradientPoint( 0.3750, math::Color(224, 224,   0)); // dirt
    renderer.AddGradientPoint( 0.7500, math::Color(128, 128, 128)); // rock
    renderer.AddGradientPoint( 1.0000, math::Color(255, 255, 255)); // snow
    renderer.EnableLight();
    renderer.SetLightContrast(3.0);
    renderer.SetLightBrightness(2.0);

    renderer.Render();

    size_t cntPixel = image.GetMemUsed();
    math::Color* colors = image.GetSlabPtr();
    uint32_t* texData = new uint32_t[cntPixel];
    for (size_t i=0 ; i!=cntPixel; ++i) {
        texData[i] = colors[i].value;
    }

    ImageHeader header(256, 256, PixelFormat::R8G8B8A8);
    return ImageView(header, 1, texData);
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
    changed |= gui::InputScalar("Seed", m_seed, gui::Step(1, 1));

    if (changed) {
        std::string error;
        auto image = Generate();
        if (!m_previewTex->Update(image, error)) {
            // spdlog::error(error);
        }
        uint32_t* texData = reinterpret_cast<uint32_t*>(image.data);
        delete []texData;
    }
}
