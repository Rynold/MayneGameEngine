#ifndef TRANSFORM_H
#define TRANSFORM_H

//The transform describes the location, rotation and size an object is in the world.

#include "Vector.h"
#include "btBulletDynamicsCommon.h"
#include "glm.hpp"
#include "VMAth.h"


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

	//void SetRotation(glm::quat quat)
	//{
	//	rotation = glm::vec4(quat.x, quat.y, quat.z, quat.w);

	//	bulletTransform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));
	//}

	void SetAxisAngleDeg(float x, float y, float z, float angle)
	{
		angle *= 3.14 / 180;
		angle /= 2;
		float sinV = sin(angle);
		float cosV = cos(angle);

		rotation = glm::vec4(x * sinV, y * sinV, z * sinV, cosV);

		bulletTransform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z,rotation.w));
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

	//void SetYawPitchRoll(float y, float p, float r, bool constrainPitch = true)
	//{
	//	yaw = y;
	//	pitch = p;
	//	roll = r;

	//	if (constrainPitch)
	//	{
	//		if (pitch > 89.0f)
	//			pitch = 89.0f;
	//		if (pitch < -89.0f)
	//			pitch = -89.0f;
	//	}

	//	UpdateVectors();
	//}

	//void UpdateVectors()
	//{
	//	glm::vec3 frontT;
	//	frontT.x = cos(MathUtil::DegToRads(yaw)) * cos(MathUtil::DegToRads(pitch));
	//	frontT.y = sin(MathUtil::DegToRads(pitch));
	//	frontT.z = sin(MathUtil::DegToRads(yaw)) * cos(MathUtil::DegToRads(pitch));
	//	front = glm::normalize(frontT);
	//	// Also re-calculate the Right and Up vector
	//	right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	//	up = glm::normalize(glm::cross(right, front));
	//}

private:
	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	btTransform bulletTransform;
};

#endif