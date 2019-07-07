#pragma once

#include <vector>

#include "engine/mesh/geometry.h"
#include "engine/material/material.h"


class Mesh {
public:
    Mesh() = default;
    ~Mesh() = default;

    void Add(const std::shared_ptr<Geometry>& geometry, const Material& material) noexcept;
    void SetModelMatrix(const glm::mat4& matrix) noexcept;

    void Draw(const std::shared_ptr<Camera>& camera) const;

private:
    glm::mat4 m_matModel = glm::mat4(1.0f);
    glm::mat3 m_matNormal = glm::mat3(1.0f);

    std::vector<std::pair<std::shared_ptr<Geometry>, Material>> m_objects;
};
