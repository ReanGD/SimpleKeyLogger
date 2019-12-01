#include "engine/mesh/material_node.h"

#include <tuple>


IndexKey::IndexKey(uint32_t shaderId, uint32_t geometryId, uint32_t materialId)
    : m_shaderId(shaderId)
    , m_geometryId(geometryId)
    , m_materialId(materialId) {

}

IndexKey::IndexKey(const IndexKey& other) noexcept
    : m_shaderId(other.m_shaderId)
    , m_geometryId(other.m_geometryId)
    , m_materialId(other.m_materialId) {

}

IndexKey::IndexKey(IndexKey&& other) noexcept
    : m_shaderId(other.m_shaderId)
    , m_geometryId(other.m_geometryId)
    , m_materialId(other.m_materialId) {
    other.m_shaderId = 0;
    other.m_geometryId = 0;
    other.m_materialId = 0;
}

IndexKey& IndexKey::operator=(const IndexKey& other) noexcept {
    m_shaderId = other.m_shaderId;
    m_geometryId = other.m_geometryId;
    m_materialId = other.m_materialId;

    return *this;
}

IndexKey& IndexKey::operator=(IndexKey&& other) noexcept {
    std::swap(m_shaderId, other.m_shaderId);
    std::swap(m_geometryId, other.m_geometryId);
    std::swap(m_materialId, other.m_materialId);
    return *this;
}

bool IndexKey::operator<(const IndexKey& other) const noexcept {
    return (std::tie(m_shaderId, m_geometryId, m_materialId) < std::tie(other.m_shaderId, other.m_geometryId, other.m_materialId));
}
IndexKey::operator bool() const noexcept {
    return ((m_shaderId == 0) && (m_geometryId == 0) && (m_materialId == 0));
}

bool IndexKey::IsEmpty() const noexcept {
    return ((m_shaderId == 0) && (m_geometryId == 0) && (m_materialId == 0));
}

MaterialNode::MaterialNode(const PrivateArg&, const std::shared_ptr<GeometryNode>& geometry, const std::shared_ptr<Material>& material)
    : m_geometry(geometry)
    , m_material(material) {
}

void MaterialNode::AttachTransformNode(const std::shared_ptr<TransformNode>& node) {
    m_transformNodes.insert(node);
}
