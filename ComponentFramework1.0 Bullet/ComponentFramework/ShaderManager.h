#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Shader.h"
#include <map>
#include <memory>

class ShaderManager
{
public:
	static ShaderManager* GetInstance();

	std::shared_ptr<Shader> Contains(const char* id);

	const char* Insert(const char* id, std::shared_ptr<Shader> shader);

	void SendLightingDataToShaders();

	int GetNumShaders();

	std::shared_ptr<Shader> GetShader(int i);

	void Delete();

	/*void SendUniformsToShader(glm::mat4& proj, glm::mat4& mod, glm::mat4& light, glm::vec3& viewPos)
	{
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "lightSpaceMatrix"), 1, GL_FALSE, light[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "CameraViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
		glUniform3f(glGetUniformLocation(sceneShader->Program, "viewPos"), viewCamera->_transform->position.x, viewCamera->_transform->position.y, viewCamera->_transform->position.z);
	}*/
	~ShaderManager();
private:
	ShaderManager();
	

	std::map<const char*,std::shared_ptr<Shader>> shaders;

	static ShaderManager* instance;

	int count;
};

#endif