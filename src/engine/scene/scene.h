#pragma once

#include "engine/mesh/mesh.h"


class Scene : Noncopyable {
public:
    Scene();
    ~Scene() = default;

    void Add(const Mesh& mesh);
    void Draw() const;

    std::shared_ptr<Camera> GetCamera() const noexcept {
        return m_camera;
    }
private:
    std::shared_ptr<Camera> m_camera;
    std::vector<Mesh> m_meshes;
};
