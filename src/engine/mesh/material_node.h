#pragma once

#include <set>
#include <memory>

#include "engine/common/noncopyable.h"


class IndexKey {
    friend class Scene;
private:
    IndexKey(uint32_t shaderId, uint32_t geometryId, uint32_t materialId);

public:
    IndexKey() = default;
    IndexKey(const IndexKey& other) noexcept;
    IndexKey(IndexKey&& other) noexcept;

    IndexKey& operator=(const IndexKey& other) noexcept;
    IndexKey& operator=(IndexKey&& other) noexcept;

    bool operator<(const IndexKey& other) const noexcept;
    operator bool() const noexcept;

    bool IsEmpty() const noexcept;

private:
    uint32_t m_shaderId = 0;
    uint32_t m_geometryId = 0;
    uint32_t m_materialId = 0;
};


class GeometryNode;
class Material;
class TransformNode;
class MaterialNode : Noncopyable {
    struct PrivateArg{};
    friend class Scene;
public:
    MaterialNode() = delete;
    MaterialNode(const PrivateArg&, const std::shared_ptr<GeometryNode>& geometry, const std::shared_ptr<Material>& material);

    void AttachTransformNode(const std::shared_ptr<TransformNode>& node);

private:
    std::shared_ptr<GeometryNode> m_geometry = nullptr;
    std::shared_ptr<Material> m_material = nullptr;
    std::set<std::shared_ptr<TransformNode>> m_transformNodes;
};
