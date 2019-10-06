#pragma once

#include <glm/mat4x4.hpp>
#include "engine/common/noncopyable.h"


class btRigidBody;
class btCollisionShape;
class PhysicalNode : Noncopyable {
    friend class Physics;
public:
    PhysicalNode() = delete;
    PhysicalNode(btCollisionShape* collisionShape, const glm::vec3& position, float bodyMass);
    ~PhysicalNode();

    void GetMatrix(glm::mat4x4& matrix) const noexcept;

private:
    btRigidBody* m_body;
};

class PhysicalShere : public PhysicalNode {
public:
    PhysicalShere(float radius, const glm::vec3& position, float bodyMass);
};

class PhysicalBox : public PhysicalNode {
public:
    PhysicalBox(const glm::vec3& boxHalfExtents, const glm::vec3& position, float bodyMass);
};

class PhysicalTerrain : public PhysicalNode {
public:
    PhysicalTerrain(uint32_t gridSize, const void* rawHeightfieldData, float heightScale, float minHeight, float maxHeight, float yPos);
};