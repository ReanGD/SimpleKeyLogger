#include "engine/physics/physics.h"

#include <glm/gtc/type_ptr.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btTriangleShape.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>


Physics::~Physics() {
    Destroy();
}

/*
static bool customMaterialCombinerCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,
    int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
    // one-sided triangles
	if (colObj1Wrap->getCollisionShape()->getShapeType() == TRIANGLE_SHAPE_PROXYTYPE) {
        auto triShape = static_cast<const btTriangleShape*>(colObj1Wrap->getCollisionShape());
        const btVector3* v = triShape->m_vertices1;
        btVector3 faceNormalLs = btCross(v[1] - v[0], v[2] - v[0]);
        faceNormalLs.normalize();
        btVector3 faceNormalWs = colObj1Wrap->getWorldTransform().getBasis() * faceNormalLs;
        float nDotF = btDot( faceNormalWs, cp.m_normalWorldOnB );
        if ( nDotF <= 0.0f ) {
            // flip the contact normal to be aligned with the face normal
            cp.m_normalWorldOnB += -2.0f * nDotF * faceNormalWs;
        }
    }

    //this return value is currently ignored, but to be on the safe side: return false if you don't calculate friction
    return false;
}
*/

btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
bool Physics::Init(std::string& /*error*/) {
	// see https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=10224
	// gContactAddedCallback = customMaterialCombinerCallback;
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -19.8, 0));

    return true;
}

void Physics::AddNode(const std::shared_ptr<PhysicalNode>& node) {
	m_dynamicsWorld->addRigidBody(node->m_body);
}

bool Physics::RayTest(const glm::vec3& from, const glm::vec3& to, glm::vec3& result) {
	btVector3 btFrom(from.x, from.y, from.z);
	btVector3 btTo(to.x, to.y, to.z);

	btCollisionWorld::ClosestRayResultCallback closestResults(btFrom, btTo);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	m_dynamicsWorld->rayTest(btFrom, btTo, closestResults);

	if (closestResults.hasHit()) {
		btVector3 p = closestResults.m_hitPointWorld;
		result.x = p.x();
		result.y = p.y();
		result.z = p.z();

		return true;
	}

	return false;
}

void Physics::Update(float deltaTime) {
    m_dynamicsWorld->stepSimulation(deltaTime, 10);

    for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);
        } else {
            trans = obj->getWorldTransform();
        }
    }
}

void Physics::Destroy() {
    int i;
    for (i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())	{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < m_collisionShapes.size(); j++) {
		btCollisionShape* shape = m_collisionShapes[j];
		m_collisionShapes[j] = 0;
		delete shape;
	}

	if (m_dynamicsWorld) {
	    delete m_dynamicsWorld;
        m_dynamicsWorld = nullptr;
    }

	if (m_solver) {
	    delete m_solver;
        m_solver = nullptr;
    }

	if (m_overlappingPairCache) {
	    delete m_overlappingPairCache;
        m_overlappingPairCache = nullptr;
    }

	if (m_dispatcher) {
	    delete m_dispatcher;
        m_dispatcher = nullptr;
    }

    if (m_collisionConfiguration) {
	    delete m_collisionConfiguration;
        m_collisionConfiguration = nullptr;
    }

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	m_collisionShapes.clear();
}
