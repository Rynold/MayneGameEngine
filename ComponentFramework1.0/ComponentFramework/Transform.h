#ifndef TRANSFORM_H
#define TRANSFORM_H

//The transform describes the location, rotation and size an object is in the world.

#include "Vector.h"
#include "glm.hpp"

using namespace MATH;

class Transform
{
public:
	Transform() : position(0.0f), scale(0.5f) {
		rotation = glm::vec4(0.0, 1.0, 0.0, 0.0);
	};
	~Transform() {} ;

	glm::vec3 position;
	glm::vec4 rotation;
	glm::vec3 scale;

	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;

	glm::vec3 worldUp;

	//Euler Angles
	float yaw;
	float pitch;
	float roll;
};

#endif