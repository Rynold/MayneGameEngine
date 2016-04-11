#ifndef TRANSFORM_H
#define TRANSFORM_H

//The transform describes the location, rotation and size an object is in the world.

#include "Vector.h"
#include "btBulletDynamicsCommon.h"
#include "glm.hpp"

using namespace MATH;

class Transform
{
public:
	Transform() : position(0.0f), scale(1.0f) {
		rotation = glm::vec4(0.0, 1.0, 0.0, 0.0);
		bulletTransform.setOrigin(btVector3(position.x, position.y,position.z));
		bulletTransform.setRotation(btQuaternion(rotation.x,rotation.y,rotation.z,rotation.w));
	}
	~Transform() {}

	void SetPosition(float x, float y, float z)
	{
		position = glm::vec3(x, y, z);

		bulletTransform.setOrigin(btVector3(x,y,z));
	}

	void SetPosition(glm::vec3 pos)
	{
		position = pos;

		bulletTransform.setOrigin(btVector3(pos.x,pos.y,pos.z));
	}

	glm::vec3 Position()
	{
		return position;
	}

	void SetRotation(float x, float y, float z, float w)
	{
		rotation = glm::vec4(x, y, z, w);

		bulletTransform.setRotation(btQuaternion(x, y, z,w));
	}

	glm::vec4 Rotation()
	{
		return rotation;
	}

	void SetScale(float x, float y, float z)
	{
		scale = glm::vec3(x, y, z);
	}

	glm::vec3 Scale()
	{
		return scale;
	}

	btTransform GetBulletTransform()
	{
		return bulletTransform;
	}

	void SetBulletTransform(const btTransform& trans)
	{
		bulletTransform = trans;
	}

	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;

	glm::vec3 worldUp;

	//Euler Angles
	float yaw;
	float pitch;
	float roll;

	//GLfloat temp[16];

private:
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	btTransform bulletTransform;
};

#endif