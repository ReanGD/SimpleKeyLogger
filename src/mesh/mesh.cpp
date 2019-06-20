#include "mesh/mesh.h"

void Mesh::Add(const std::shared_ptr<Geometry>& geometry, const Material& material) noexcept {
    m_objects.push_back(std::make_pair(geometry, material));
}

void Mesh::Draw(const std::shared_ptr<Camera>& camera, const glm::mat4& matWorld) const {
    for(const auto& pair: m_objects) {
        pair.second.Bind(camera, matWorld);
        pair.first->Bind();
        pair.first->Draw();
        pair.first->Unbind();
        pair.second.Unbind();
    }
}
