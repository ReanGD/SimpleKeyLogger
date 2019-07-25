#pragma once

#include "engine/mesh/mesh.h"


class Scene : Noncopyable {
public:
    Scene();
    ~Scene() = default;

    void Add(const Mesh& mesh);
    void Draw();
    void DrawWithMaterial(const Material& material);

    uint32_t GetCountTriangles() const noexcept {
        return m_countTriangles;
    }
    std::shared_ptr<Camera> GetCamera() const noexcept {
        return m_camera;
    }
private:
    uint32_t m_countTriangles = 0;
    std::shared_ptr<Camera> m_camera;
    std::vector<Mesh> m_meshes;
};
