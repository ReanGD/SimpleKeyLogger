#pragma once

#include <vector>

#include "mesh/geometry.h"
#include "material/material.h"


class Mesh {
public:
    Mesh() = default;
    ~Mesh() = default;

    void Add(const Geometry& geometry, const Material& material) noexcept;
    void Draw(const std::shared_ptr<Camera>& camera, const glm::mat4& matWorld) const;

private:
    std::vector<std::pair<Geometry, Material>> m_objects;
};
