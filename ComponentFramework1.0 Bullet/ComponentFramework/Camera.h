#pragma once
#ifndef CAMERA_H
#define CAMERA_H
// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include "GameObject.h"
#include "NetworkListener.h"
#include "VMAth.h"

using namespace MATH;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD = 0,
	RIGHT = 1,
	BACKWARD = 2,
	LEFT = 3,
	UP = 4,
	DOWN = 5
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera : public GameObject, public NetworkListener
{
public:
	
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	bool moveForward;
	bool moveBackWard;
	bool moveRight;
	bool moveLeft;
	bool moveUp;
	bool moveDown;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		//return MMath::lookAt(_transform->position, _transform->position + _transform->front, _transform->up);
		return glm::lookAt(_transform->Position(), _transform->Position() + _transform->front, _transform->up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessMovement(Camera_Movement direction, GLfloat deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset);
	
	virtual void Update(float deltaTime);
	
	virtual void ReceiveNetworkSDLEvent(const SDL_Event& sdlEvent);
	
private:

};

#endif CAMERA