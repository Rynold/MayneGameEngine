#pragma once
#ifndef BULLETPHYSICS_H
#define BULLETPHYSICS_H

#include "btBulletDynamicsCommon.h"
#include "GLDebugDrawer.h"

class btCollisionShape;

class BulletPhysics
{

public:

	BulletPhysics();
	~BulletPhysics();

	void initPhysics();

	btDynamicsWorld*		m_dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
	
	class btBroadphaseInterface*	m_overlappingPairCache;

	class btCollisionDispatcher*	m_dispatcher;

	class btConstraintSolver*	m_constraintSolver;

	class btDefaultCollisionConfiguration* m_collisionConfiguration;

	class btTriangleIndexVertexArray*	m_indexVertexArrays;
};

#endif