#include "engine/scene/scene.h"

#include <glm/gtc/constants.hpp>

#include "engine/camera/camera.h"
#include "engine/material/material.h"
#include "engine/scene/geometry_node.h"


Scene::Scene() {
    m_camera = std::make_shared<Camera>(glm::quarter_pi<float>(), 0.1f, 100.0);
}

std::shared_ptr<MaterialNode> Scene::CreateMaterialNode(const std::shared_ptr<Material>& material, const std::shared_ptr<GeometryNode>& geometry) {
    IndexKey key(material->GetShaderId(), geometry->GetId(), material->GetId());
    if (auto it = m_index.find(key); it != m_index.cend()) {
        return it->second;
    }

    auto result = std::make_shared<MaterialNode>(MaterialNode::PrivateArg{}, geometry, material);
    m_index[key] = result;
    return result;
}

void Scene::Update() {
    m_countTriangles = 0;
    for(const auto& [_, value]: m_index) {
        value->m_transformNodes.clear();
    }
    UpdateGraph();
}

void Scene::Draw() {
    uint32_t lastShaderId = 0;
    uint32_t lastGeometryId = 0;
    for(const auto& [key, value]: m_index) {
        if (key.m_shaderId != lastShaderId) {
            value->m_material->BindShader();
            lastShaderId = key.m_shaderId;
        }
        if (key.m_geometryId != lastGeometryId) {
            value->m_geometry->Bind();
            lastGeometryId = key.m_geometryId;
        }
        for (const auto& transformNode: value->m_transformNodes) {
            value->m_material->BindUniforms(m_camera, transformNode->GetTotalTransform(), transformNode->GetTotalNormalMatrix());
            m_countTriangles += value->m_geometry->Draw();
        }
    }
}

void Scene::DrawWithMaterial(const std::shared_ptr<Material>& material) {
    material->BindShader();
    for(const auto& [key, value]: m_index) {
        for (const auto& transformNode: value->m_transformNodes) {
            material->BindUniforms(m_camera, transformNode->GetTotalTransform(), transformNode->GetTotalNormalMatrix());
            value->m_geometry->Bind();
            m_countTriangles += value->m_geometry->Draw();
            value->m_geometry->Unbind();
        }
    }
}
