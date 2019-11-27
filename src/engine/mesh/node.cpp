#include "engine/mesh/node.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/mesh/mesh.h"
#include "engine/camera/camera.h"
#include "engine/material/material.h"
#include "engine/physics/physical_node.h"


void Node::Add(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) {
    m_meshes.push_back(std::make_pair(mesh, material));
}

void Node::SetPhysicalNode(const std::shared_ptr<PhysicalNode>& node) noexcept {
    m_physicalNode = node;
}

void Node::SetModelMatrix(const glm::mat4& matrix) noexcept {
    m_matModel = matrix;
    m_matNormal = glm::inverseTranspose(glm::mat3(m_matModel));
}

void Node::Update() {
    if (m_physicalNode) {
        m_physicalNode->GetMatrix(m_matModel);
        m_matNormal = glm::inverseTranspose(glm::mat3(m_matModel));
    }
}

uint32_t Node::Draw(const std::shared_ptr<Camera>& camera) const {
    uint32_t count = 0;
    for(const auto& pair: m_meshes) {
        pair.second->Bind(camera, m_matModel, m_matNormal);
        pair.first->Bind();
        count += pair.first->Draw();
        pair.first->Unbind();
        pair.second->Unbind();
    }

    return count;
}

uint32_t Node::DrawWithMaterial(const std::shared_ptr<Camera>& camera, const std::shared_ptr<Material>& material) const {
    uint32_t count = 0;
    for(const auto& pair: m_meshes) {
        material->Bind(camera, m_matModel, m_matNormal);
        pair.first->Bind();
        count += pair.first->Draw();
        pair.first->Unbind();
        material->Unbind();
    }

    return count;
}
