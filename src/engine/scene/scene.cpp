#include "engine/scene/scene.h"

Scene::Scene() {
    m_camera = std::make_shared<Camera>(glm::quarter_pi<float>(), 0.1f, 100.0);
}

size_t Scene::Add(const Mesh& mesh) {
    const size_t index = m_meshes.size();
    m_meshes.push_back(mesh);

    return index;
}

void Scene::SetModelMatrix(size_t index, const glm::mat4& matrix) noexcept {
    if (index < m_meshes.size()) {
        m_meshes[index].SetModelMatrix(matrix);
    }
}

void Scene::Draw() {
    m_countTriangles = 0;
    for(const auto& mesh: m_meshes) {
        m_countTriangles += mesh.Draw(m_camera);
    }
}

void Scene::DrawWithMaterial(const Material& material) {
    m_countTriangles = 0;
    for(const auto& mesh: m_meshes) {
        m_countTriangles += mesh.DrawWithMaterial(m_camera, material);
    }
}
