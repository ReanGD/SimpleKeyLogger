#pragma once

#include <filesystem>

#include "engine/material/image.h"
#include "engine/common/noncopyable.h"
#include "engine/physics/physical_node.h"


class Texture;
class Heightmap : Noncopyable {
public:
    enum class Quality : uint8_t {
        Fast = 0,
        Std,
        Best,
        Count,
    };
public:
    Heightmap() = default;
    ~Heightmap() = default;

    void Create();
    std::shared_ptr<PhysicalNode> Load(const std::filesystem::path& path);

    ImageView Generate() const;
    void DrawSettings();

    std::shared_ptr<Texture> GetPreview() const noexcept {
        return m_previewTex;
    }

private:
    double m_frequency = 1.0;              // DEFAULT_PERLIN_FREQUENCY
    double m_lacunarity = 2.0;             // DEFAULT_PERLIN_LACUNARITY
    Quality m_noiseQuality = Quality::Std; // DEFAULT_PERLIN_QUALITY
    uint8_t m_octaveCount = 6;             // DEFAULT_PERLIN_OCTAVE_COUNT
    double m_persistence = 0.5;            // DEFAULT_PERLIN_PERSISTENCE
    int32_t m_seed = 0;                    // DEFAULT_PERLIN_SEED

    std::shared_ptr<Texture> m_previewTex;
};
