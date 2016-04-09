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
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		_transform->SetPosition(position);
		_transform->worldUp = up;
		_transform->yaw = yaw;
		_transform->pitch = pitch;
		_transform->front = glm::vec3(0.0f, 0.0f, -1.0f);

		moveBackWard = false;
		moveForward = false;
		moveLeft = false;
		moveRight = false;
		moveUp = false;
		moveDown = false;

		this->updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		_transform->SetPosition(posX, posY, posZ);
		_transform->worldUp = glm::vec3(upX, upY, upZ);
		_transform->yaw = yaw;
		_transform->pitch = pitch;
		_transform->front = glm::vec3(0.0f, 0.0f, -1.0f);

		moveBackWard = false;
		moveForward = false;
		moveLeft = false;
		moveRight = false;
		moveUp = false;
		moveDown = false;

		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		//return MMath::lookAt(_transform->position, _transform->position + _transform->front, _transform->up);
		return glm::lookAt(_transform->Position(), _transform->Position() + _transform->front, _transform->up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessMovement(Camera_Movement direction, GLfloat deltaTime)
	{
		//std::cout << "Camera Movement" << std::endl;
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == Camera_Movement::FORWARD)
			_transform->SetPosition(_transform->Position() += _transform->front * velocity);
		if (direction == Camera_Movement::BACKWARD)
			_transform->SetPosition(_transform->Position() -= _transform->front * velocity);
		if (direction == Camera_Movement::LEFT)
			_transform->SetPosition(_transform->Position() -= _transform->right * velocity);
		if (direction == Camera_Movement::RIGHT)
			_transform->SetPosition(_transform->Position() += _transform->right * velocity);
		if (direction == Camera_Movement::UP)
			_transform->SetPosition(_transform->Position() += _transform->worldUp * velocity);
		if (direction == Camera_Movement::DOWN)
			_transform->SetPosition(_transform->Position() -= _transform->worldUp * velocity);
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		//std::cout << "xoffset: " << xoffset << " / yoffset: " << yoffset << std::endl;

		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		_transform->yaw += xoffset;
		_transform->pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (_transform->pitch > 89.0f)
				_transform->pitch = 89.0f;
			if (_transform->pitch < -89.0f)
				_transform->pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(MathUtil::DegToRads(_transform->yaw)) * cos(MathUtil::DegToRads(_transform->pitch));
		front.y = sin(MathUtil::DegToRads(_transform->pitch));
		front.z = sin(MathUtil::DegToRads(_transform->yaw)) * cos(MathUtil::DegToRads(_transform->pitch));
		_transform->front = glm::normalize(front); //glm::normalize(front);
		// Also re-calculate the Right and Up vector
		_transform->right = glm::normalize(glm::cross(_transform->front, _transform->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		_transform->up = glm::normalize(glm::cross(_transform->right, _transform->front));
	}
	
	virtual void Update(float deltaTime) override
	{
		if (moveBackWard)
			ProcessMovement(BACKWARD, deltaTime);
		if (moveForward)
			ProcessMovement(FORWARD, deltaTime);
		if (moveLeft)
			ProcessMovement(LEFT, deltaTime);
		if (moveRight)
			ProcessMovement(RIGHT, deltaTime);
		if (moveUp)
			ProcessMovement(UP, deltaTime);
		if (moveDown)
			ProcessMovement(DOWN, deltaTime);
	}

	virtual void ReceiveNetworkSDLEvent(const SDL_Event& sdlEvent) override
	{
		switch (sdlEvent.type) {
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W)
					moveForward = true;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A)
					moveLeft = true;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S)
					moveBackWard = true;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D)
					moveRight = true;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E)
					moveUp = true;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q)
					moveDown = true;

				break;
			case SDL_KEYUP:
				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W)
					moveForward = false;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A)
					moveLeft = false;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S)
					moveBackWard = false;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D)
					moveRight = false;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E)
					moveUp = false;

				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q)
					moveDown = false;
				break;
			default:
				break;
		}
	}

private:

};

#endif CAMERA