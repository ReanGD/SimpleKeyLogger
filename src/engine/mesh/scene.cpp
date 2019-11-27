#include "engine/mesh/scene.h"

#include <glm/gtc/constants.hpp>

#include "engine/mesh/mesh.h"
#include "engine/camera/camera.h"
#include "engine/material/material.h"


Scene::Scene() {
    m_camera = std::make_shared<Camera>(glm::quarter_pi<float>(), 0.1f, 100.0);
}

void Scene::Add(const Node& node) {
    m_nodes.push_back(node);
}

void Scene::Update() {
    for(auto& mesh: m_nodes) {
        mesh.Update();
    }
}

void Scene::Draw() {
    m_countTriangles = 0;
    for(const auto& mesh: m_nodes) {
        m_countTriangles += mesh.Draw(m_camera);
    }
}

void Scene::DrawWithMaterial(const std::shared_ptr<Material>& material) {
    m_countTriangles = 0;
    for(const auto& mesh: m_nodes) {
        m_countTriangles += mesh.DrawWithMaterial(m_camera, material);
    }
}
