#pragma once

#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>

#include "engine/common/noncopyable.h"


// class PhysicalNode;
class MaterialNode;
class TransformNode : Noncopyable, public std::enable_shared_from_this<TransformNode> {
public:
    TransformNode() = default;
    TransformNode(const glm::mat4& transform, const std::shared_ptr<TransformNode>& parent = nullptr);
    ~TransformNode() = default;

    std::shared_ptr<TransformNode> Clone(const glm::mat4& transform);
    std::shared_ptr<TransformNode> Clone(const std::shared_ptr<TransformNode>& parent);

    std::shared_ptr<TransformNode> NewChild(const glm::mat4& transform = glm::mat4(1));
    std::shared_ptr<TransformNode> NewChild(const std::shared_ptr<MaterialNode>& materialNode, const glm::mat4& transform = glm::mat4(1));

    void AddChild(const std::shared_ptr<TransformNode>& node);

    void SetTransform(const glm::mat4& transform);
    const glm::mat4& GetBaseTransform() const noexcept { return m_baseTransform; }
    const glm::mat4& GetTotalTransform() const noexcept { return m_totalTransform; }
    const glm::mat3& GetTotalNormalMatrix() const noexcept { return m_totalNormalMatrix; }

    void Update();

private:
    std::weak_ptr<TransformNode> m_parent;
    std::vector<std::shared_ptr<TransformNode>> m_children;
    bool m_isDirty = true;
    glm::mat4 m_baseTransform = glm::mat4(1);
    glm::mat4 m_totalTransform = glm::mat4(1);
    glm::mat3 m_totalNormalMatrix = glm::mat3(1);
    // std::shared_ptr<PhysicalNode> m_physicalNode = nullptr;
};

class TransformGraph {
public:
    TransformGraph();
    ~TransformGraph() = default;

    std::shared_ptr<TransformNode> NewChild(const glm::mat4& transform = glm::mat4(1));
    std::shared_ptr<TransformNode> NewChild(const std::shared_ptr<MaterialNode>& materialNode, const glm::mat4& transform = glm::mat4(1));
    void AddChild(const std::shared_ptr<TransformNode>& node);

    void UpdateGraph();
private:
    std::shared_ptr<TransformNode> m_root = nullptr;
};
