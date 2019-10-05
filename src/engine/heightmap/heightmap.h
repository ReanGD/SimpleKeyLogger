#pragma once

#include <filesystem>

#include "engine/common/noncopyable.h"
#include "engine/physics/physical_node.h"


class Heightmap : Noncopyable {
public:
    Heightmap() = default;
    ~Heightmap() = default;

    std::shared_ptr<PhysicalNode> Load(const std::filesystem::path& path, std::string& error) noexcept;
};
