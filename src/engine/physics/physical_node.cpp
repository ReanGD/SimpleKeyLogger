#include "engine/physics/physical_node.h"

#include <glm/gtc/type_ptr.hpp>
#include <bullet/LinearMath/btDefaultMotionState.h>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>
#include <bullet/BulletCollision/CollisionShapes/btSphereShape.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>


PhysicalNode::PhysicalNode(btCollisionShape* collisionShape, const glm::vec3& position, float bodyMass) {
    btQuaternion rotation;
    rotation.setEulerZYX(0, 0, 0);
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, btVector3(position.x, position.y, position.z)));

	bool isDynamic = (bodyMass != 0.f);

    btVector3 bodyInertia(0, 0, 0);
    if (isDynamic) {
        collisionShape->calculateLocalInertia(bodyMass, bodyInertia);
    }

    btRigidBody::btRigidBodyConstructionInfo rbInfo(bodyMass, motionState, collisionShape, bodyInertia);
    rbInfo.m_restitution = 0.5;
    rbInfo.m_friction = 0.5;

	m_body = new btRigidBody(rbInfo);
}

PhysicalNode::~PhysicalNode() {
    // if (m_body) {
    //     auto* collisionShape = m_body->getCollisionShape();
    //     if (collisionShape) {
    //         delete collisionShape;
    //     }
    //     auto* motionState = m_body->getMotionState();
    //     if (motionState) {
    //         delete motionState;
    //     }
    //     delete m_body;
    //     m_body = nullptr;
    // }
}

void PhysicalNode::GetMatrix(glm::mat4x4& matrix) const noexcept {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    trans.getOpenGLMatrix(glm::value_ptr(matrix));
}

PhysicalShere::PhysicalShere(float radius, const glm::vec3& position, float bodyMass)
    : PhysicalNode(new btSphereShape(radius), position, bodyMass) {
}

PhysicalBox::PhysicalBox(const glm::vec3& boxHalfExtents, const glm::vec3& position, float bodyMass)
    : PhysicalNode(new btBoxShape(btVector3(boxHalfExtents.x, boxHalfExtents.y, boxHalfExtents.z)), position, bodyMass) {
}
