#include "engine/scene/scene.h"

Scene::Scene() {
    m_camera = std::make_shared<Camera>(glm::quarter_pi<float>(), 0.1f, 100.0);
}

void Scene::Add(const Mesh& mesh) {
    m_meshes.push_back(mesh);
}

void Scene::Draw() const {
    for(const auto& mesh: m_meshes) {
        mesh.Draw(m_camera);
    }
}
