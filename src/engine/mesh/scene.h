#pragma once

#include "engine/mesh/node.h"
#include "engine/common/noncopyable.h"


class Scene : Noncopyable {
public:
    Scene();
    ~Scene() = default;

    void Add(const Node& node);

    void Update();
    void Draw();
    void DrawWithMaterial(const std::shared_ptr<Material>& material);

    uint32_t GetCountTriangles() const noexcept {
        return m_countTriangles;
    }
    std::shared_ptr<Camera> GetCamera() const noexcept {
        return m_camera;
    }
private:
    uint32_t m_countTriangles = 0;
    std::shared_ptr<Camera> m_camera;
    std::vector<Node> m_nodes;
};
