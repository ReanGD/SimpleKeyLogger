#pragma once

#include <vector>

#include "engine/mesh/geometry.h"
#include "engine/material/material.h"


class Mesh {
public:
    Mesh() = default;
    ~Mesh() = default;

    void Add(const std::shared_ptr<Geometry>& geometry, const Material& material) noexcept;
    void Draw(const std::shared_ptr<Camera>& camera, const glm::mat4& matWorld) const;

private:
    std::vector<std::pair<std::shared_ptr<Geometry>, Material>> m_objects;
};
