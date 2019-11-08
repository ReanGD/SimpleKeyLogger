#include "engine/heightmap/heightmap.h"

#include <noise.h>

#include "engine/gui/widgets.h"
#include "engine/common/exception.h"
#include "engine/material/texture_manager.h"
#include "middleware/node_editor/noiseutils.h"

static const char* QualityItems[] = {"Fast", "Std", "Best"};
static utils::RendererImage renderer;

static noise::NoiseQuality ToNoiseType(const Heightmap::Quality value) {
    switch (value) {
    case Heightmap::Quality::Fast: return noise::NoiseQuality::QUALITY_FAST;
    case Heightmap::Quality::Std: return noise::NoiseQuality::QUALITY_STD;
    case Heightmap::Quality::Best: return noise::NoiseQuality::QUALITY_BEST;
    default: return noise::NoiseQuality::QUALITY_STD;
    }
}

void Heightmap::Create() {
    auto image = Generate();
    m_previewTex = TextureManager::Get().Create(image);
}

std::shared_ptr<PhysicalNode> Heightmap::Load(const std::filesystem::path& path) {
    auto fullPath = TextureManager::Get().GetFullPath(path);
    Image image(fullPath.c_str(), true);

    auto header = image.view.header;
    if (header.format != PixelFormat::R8G8B8) {
        throw EngineError("wrong image ({}) format for heightmap, expected: {}, actual: '{}'",
            fullPath.c_str(), ToStr(PixelFormat::R8G8B8), ToStr(header.format));
    }

    if (header.width != header.height) {
        throw EngineError("wrong image ({}) size for heightmap, expected: width == height, actual: width = {}, height = {}",
            fullPath.c_str(), header.width, header.height);
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

ImageView Heightmap::Generate() const {
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

    renderer.SetSourceModule(planeShape);
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

    return renderer.Render();
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
        m_previewTex->Update(Generate());
    }
}
