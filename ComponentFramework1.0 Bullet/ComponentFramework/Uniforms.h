#include <glm.hpp>
#include <GL\glew.h>


class Uniforms{
	glm::mat4* projectionMatrix;
	glm::mat4* cameraViewMatrix;

	glm::mat4* lightSpaceMatrix;
	glm::vec3* viewPos;

	GLuint* skybox;
	GLuint* shadowMap;

public:

	Uniforms()
	{}

	~Uniforms()
	{
		delete projectionMatrix;
		projectionMatrix = nullptr;

		delete cameraViewMatrix;
		cameraViewMatrix = nullptr;

		delete lightSpaceMatrix;
		lightSpaceMatrix = nullptr;

		delete viewPos;
		viewPos = nullptr;

		delete skybox;
		skybox = nullptr;

		delete shadowMap;
		shadowMap = nullptr;
	}

	glm::mat4* GetProjectionMatrix()
	{
		return projectionMatrix;
	}

	glm::mat4* GetCameraViewMatrix()
	{
		return cameraViewMatrix;
	}

	glm::mat4* GetLightSpaceMatrix()
	{
		return lightSpaceMatrix;
	}

	glm::vec3* GetViewPos()
	{
		return viewPos;
	}

	GLuint* GetSkybox()
	{
		return skybox;
	}

	GLuint* GetShadowMap()
	{
		return shadowMap;
	}

};