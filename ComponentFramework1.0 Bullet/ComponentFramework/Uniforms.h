#include <glm.hpp>
#include <GL\glew.h>


class Uniforms{
	static glm::mat4 projectionMatrix;
	static glm::mat4 cameraViewMatrix;

	static glm::mat4 lightSpaceMatrix;
	static glm::vec3 viewPos;

	static GLuint skybox;
	static GLuint shadowMap;

public:

	Uniforms()
	{}

	~Uniforms()
	{
		/*delete projectionMatrix;
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
		shadowMap = nullptr;*/
	}

	static inline void SetProjectionMatrix(glm::mat4 m){
		projectionMatrix = m;
	}

	static inline glm::mat4 GetProjectionMatrix()
	{
		return projectionMatrix;
	}

	static inline void SetCameraViewMatrix(glm::mat4 m){
		cameraViewMatrix = m;
	}

	static inline glm::mat4 GetCameraViewMatrix()
	{
		return cameraViewMatrix;
	}

	static inline void SetLightSpaceMatrix(glm::mat4 m){
		lightSpaceMatrix = m;
	}

	static inline glm::mat4 GetLightSpaceMatrix()
	{
		return lightSpaceMatrix;
	}

	static inline void SetViewPos(glm::vec3 v){
		viewPos = v;
	}

	static inline glm::vec3 GetViewPos()
	{
		return viewPos;
	}

	static inline void SetSkyBox(GLuint i)
	{
		skybox = i;
	}

	static 	inline GLuint GetSkybox()
	{
		return skybox;
	}

	static 	inline void SetShadowMap(GLuint i)
	{
		shadowMap = i;
	}

	static 	inline GLuint GetShadowMap()
	{
		return shadowMap;
	}

};