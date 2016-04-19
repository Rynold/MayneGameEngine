#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Shader.h"
#include <map>

class ShaderManager
{
public:
	static ShaderManager* GetInstance();

	Shader* Contains(const char* id);

	const char* Insert(const char* id, Shader* shader);

	void SendLightingDataToShaders();

	int GetNumShaders();

	Shader* GetShader(int i);

	/*void SendUniformsToShader(glm::mat4& proj, glm::mat4& mod, glm::mat4& light, glm::vec3& viewPos)
	{
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "lightSpaceMatrix"), 1, GL_FALSE, light[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "CameraViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
		glUniform3f(glGetUniformLocation(sceneShader->Program, "viewPos"), viewCamera->_transform->position.x, viewCamera->_transform->position.y, viewCamera->_transform->position.z);
	}*/

private:
	ShaderManager();
	~ShaderManager();

	std::map<const char*,Shader*> shaders;

	static ShaderManager* instance;

	int count;
};

#endif