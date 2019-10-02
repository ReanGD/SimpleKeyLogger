#pragma once

#include <string>
#include <glm/mat4x4.hpp>

#include "engine/common/noncopyable.h"


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
    void Update(float deltaTime);
    glm::mat4x4 GetMatrix() const noexcept {
        return m_matrix;
    }

private:
    void Destroy();

private:
    glm::mat4x4 m_matrix = glm::mat4x4(1.0f);

    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btBroadphaseInterface* m_overlappingPairCache;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
};
