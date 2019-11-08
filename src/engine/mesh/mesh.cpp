#include "engine/mesh/mesh.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/physics/physical_node.h"


void Mesh::Add(const std::shared_ptr<Geometry>& geometry, const Material& material) {
    m_objects.push_back(std::make_pair(geometry, material));
}

void Mesh::SetPhysicalNode(const std::shared_ptr<PhysicalNode>& node) noexcept {
    m_physicalNode = node;
}

void Mesh::SetModelMatrix(const glm::mat4& matrix) noexcept {
    m_matModel = matrix;
    m_matNormal = glm::inverseTranspose(glm::mat3(m_matModel));
}

void Mesh::Update() {
    if (m_physicalNode) {
        m_physicalNode->GetMatrix(m_matModel);
        m_matNormal = glm::inverseTranspose(glm::mat3(m_matModel));
    }
}

uint32_t Mesh::Draw(const std::shared_ptr<Camera>& camera) const {
    uint32_t count = 0;
    for(const auto& pair: m_objects) {
        pair.second.Bind(camera, m_matModel, m_matNormal);
        pair.first->Bind();
        count += pair.first->Draw();
        pair.first->Unbind();
        pair.second.Unbind();
    }

    return count;
}

uint32_t Mesh::DrawWithMaterial(const std::shared_ptr<Camera>& camera, const Material& material) const {
    uint32_t count = 0;
    for(const auto& pair: m_objects) {
        material.Bind(camera, m_matModel, m_matNormal);
        pair.first->Bind();
        count += pair.first->Draw();
        pair.first->Unbind();
        material.Unbind();
    }

    return count;
}
