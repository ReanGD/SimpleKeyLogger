#include "engine/scene/transform_graph.h"

#include <glm/gtc/matrix_inverse.hpp>

#include "engine/scene/material_node.h"
// #include "engine/physics/physical_node.h"

TransformNode::TransformNode(const glm::mat4& transform)
    : m_baseTransform(transform) {

}

TransformNode::TransformNode(const glm::mat4& transform, const std::weak_ptr<TransformNode>& parent)
    : m_parent(parent)
    , m_baseTransform(transform) {

}

std::shared_ptr<TransformNode> TransformNode::Clone(const glm::mat4& transform) const {
    auto node = std::make_shared<TransformNode>(transform);
    if (auto matNode = m_materialNode.lock()) {
        node->m_materialNode = m_materialNode;
    }

    node->m_children.reserve(m_children.size());
    for (const auto& child : m_children) {
        node->AddChild(child->Clone(node));
    }

    return node;
}

std::shared_ptr<TransformNode> TransformNode::Clone(const std::weak_ptr<TransformNode>& parent) const {
    auto node = std::make_shared<TransformNode>(m_baseTransform, parent);
    if (auto matNode = m_materialNode.lock()) {
        node->m_materialNode = m_materialNode;
    }

    node->m_children.reserve(m_children.size());
    for (const auto& child : m_children) {
        node->AddChild(child->Clone(node));
    }

    return node;
}

std::shared_ptr<TransformNode> TransformNode::NewChild(const glm::mat4& transform) {
    auto node = std::make_shared<TransformNode>(transform, shared_from_this());
    m_children.push_back(node);

    return node;
}

std::shared_ptr<TransformNode> TransformNode::NewChild(const std::shared_ptr<MaterialNode>& materialNode, const glm::mat4& transform) {
    auto node = std::make_shared<TransformNode>(transform, shared_from_this());
    node->m_materialNode = materialNode;
    m_children.push_back(node);

    return node;
}

void TransformNode::AddChild(const std::shared_ptr<TransformNode>& node) {
    node->m_parent = shared_from_this();
    m_children.push_back(node);
}

void TransformNode::SetTransform(const glm::mat4& transform) {
    m_isDirty = true;
    m_baseTransform = transform;
}

void TransformNode::Update() {
    if (m_isDirty) {
        if (auto parent = m_parent.lock()) {
            m_totalTransform = parent->m_totalTransform * m_baseTransform;
            m_totalNormalMatrix = glm::inverseTranspose(glm::mat3(m_totalTransform));
            m_isDirty = false;
        }
    }

    if (auto matNode = m_materialNode.lock()) {
        matNode->AttachTransformNode(shared_from_this());
    }

    for (auto& node : m_children) {
        node->Update();
    }
}

TransformGraph::TransformGraph()
    : m_root(std::make_shared<TransformNode>(glm::mat4(1), std::make_shared<TransformNode>())) {

}

std::shared_ptr<TransformNode> TransformGraph::NewChild(const glm::mat4& transform) {
    return m_root->NewChild(transform);
}

std::shared_ptr<TransformNode> TransformGraph::NewChild(const std::shared_ptr<MaterialNode>& materialNode, const glm::mat4& transform) {
    return m_root->NewChild(materialNode, transform);
}

void TransformGraph::AddChild(const std::shared_ptr<TransformNode>& node) {
    m_root->AddChild(node);
}

void TransformGraph::UpdateGraph() {
    m_root->Update();
}
