#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) 
	: MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
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

	_transform->UpdateVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
	: MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
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

	_transform->UpdateVectors();
	//this->updateCameraVectors();
}

void Camera::ProcessMovement(Camera_Movement direction, GLfloat deltaTime)
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

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	//std::cout << "xoffset: " << xoffset << " / yoffset: " << yoffset << std::endl;

	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	_transform->yaw += xoffset;
	_transform->pitch += yoffset;

	_transform->SetYawPitchRoll(_transform->yaw + xoffset, _transform->pitch + yoffset, _transform->roll);
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		this->Zoom -= yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= 45.0f)
		this->Zoom = 45.0f;
}

void Camera::Update(float deltaTime)
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

void Camera::ReceiveNetworkSDLEvent(const SDL_Event& sdlEvent)
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