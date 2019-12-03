#pragma once

#include <map>
#include "engine/scene/material_node.h"
#include "engine/scene/transform_graph.h"


class Camera;
class Material;
class GeometryNode;
class Scene : Noncopyable, public TransformGraph {
public:
    Scene();
    ~Scene() = default;

    std::shared_ptr<MaterialNode> CreateMaterialNode(const std::shared_ptr<Material>& material, const std::shared_ptr<GeometryNode>& geometry);

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
    std::map<IndexKey, std::shared_ptr<MaterialNode>> m_index;
};
