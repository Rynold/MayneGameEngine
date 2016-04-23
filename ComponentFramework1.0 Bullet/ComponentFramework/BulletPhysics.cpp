#include "BulletPhysics.h"

BulletPhysics::BulletPhysics()
{
	initPhysics();
}

BulletPhysics::~BulletPhysics()
{
	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	delete m_dynamicsWorld->getDebugDrawer();
	//delete collision shapes
	for (int j = m_collisionShapes.size() - 1; j >= 0; j--)
	{
		delete m_collisionShapes[j];
		m_collisionShapes.pop_back();
	}
	//m_collisionShapes.clear();

	//delete dynamics world
	delete m_dynamicsWorld;

	//delete solver
	delete m_constraintSolver;

	//delete broadphase
	delete m_overlappingPairCache;

	//delete dispatcher
	delete m_dispatcher;

	delete m_collisionConfiguration;

	delete m_indexVertexArrays;
}

void BulletPhysics::initPhysics()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	btVector3 worldMin(-1000, -1000, -1000);
	btVector3 worldMax(1000, 1000, 1000);
	m_overlappingPairCache = new btAxisSweep3(worldMin, worldMax);
	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_constraintSolver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0,-15,0));

	GLDebugDrawer* drawer = new GLDebugDrawer();
	drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	m_dynamicsWorld->setDebugDrawer(drawer);
}