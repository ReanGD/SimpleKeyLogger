#pragma once

#include <memory>

#include "engine/common/noncopyable.h"
#include "engine/physics/physical_node.h"


class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class Physics : Noncopyable {
public:
    Physics() = default;
    ~Physics();

    void Init();
    void AddNode(const std::shared_ptr<PhysicalNode>& node);
    bool RayTest(const glm::vec3& from, const glm::vec3& to, glm::vec3& result);

    void Update(float deltaTime);

private:
    void Destroy();

private:
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btBroadphaseInterface* m_overlappingPairCache;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
};
