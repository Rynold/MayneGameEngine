#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Component.h"
#include "BulletPhysics.h"


class RigidBody: public Component
{
public:
	// The Default constructic will create a static cube.
	RigidBody(){
		shape = new btBoxShape(btVector3(1,1,1));


		btVector3 localInertia(0.0, 0.0, 0.0);
		btScalar mass(0.5);
		shape->calculateLocalInertia(mass, localInertia);

	}
	~RigidBody(){

	}

	btRigidBody* body;
	float mass;
	btCollisionShape* shape;

	void setShape(btCollisionShape* shape)
	{
		this->shape = shape;
	}
};

#endif