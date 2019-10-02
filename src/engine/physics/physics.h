#pragma once

#include <string>

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

    bool Init(std::string& error);
    void AddNode(PhysicalNode* node);

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
