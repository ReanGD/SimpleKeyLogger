#pragma once

#include <filesystem>

#include "engine/common/noncopyable.h"
#include "engine/physics/physical_node.h"


class Texture;
class Heightmap : Noncopyable {
public:
    Heightmap() = default;
    ~Heightmap() = default;

    bool Create(std::string& error) noexcept;
    std::shared_ptr<PhysicalNode> Load(const std::filesystem::path& path, std::string& error) noexcept;

    std::shared_ptr<Texture> GetPreview() const noexcept {
        return m_previewTex;
    }

private:
    std::shared_ptr<Texture> m_previewTex;
};
