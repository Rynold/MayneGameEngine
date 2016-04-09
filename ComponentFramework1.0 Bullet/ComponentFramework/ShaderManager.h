#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Shader.h"
#include <map>

class ShaderManager
{
public:
	static ShaderManager* GetInstance()
	{
		if (!instance)
			instance = new ShaderManager();
			
		return instance;
	}

	Shader* Contains(int id)
	{
		if (shaders.find(id) != shaders.end())
			return shaders.find(id)->second;
		else
			return nullptr;
	}

	int Insert(int id, Shader* shader)
	{
		if (id == -1)
		{
			/*if (Contains(shader->id))
				return id;*/

			count++;
			shaders.insert(std::pair<int, Shader*>(count, shader));
			shader->id = count;
		}
		else
		{
			if (Contains(id))
				std::cout << "*** WARNING *** :: Shader Manager already contains a shadder with that Id. This shadder will not be added to the map." << std::endl;
			shaders.insert(std::pair<int, Shader*>(count, shader));
		}
		return shader->id;
	}

	void SendLightingDataToShaders()
	{
		for (auto it = shaders.begin(); it != shaders.end(); it++)
		{
			it->second->Use();
			glUniform3f(glGetUniformLocation(it->second->Program, "dirLight.position"), -4.0, 8.0, 2.0);
			glUniform3f(glGetUniformLocation(it->second->Program, "dirLight.direction"), 0.0f, -1.0f, -0.3f);
			glUniform3f(glGetUniformLocation(it->second->Program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(it->second->Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
			glUniform3f(glGetUniformLocation(it->second->Program, "dirLight.specular"), 0.6f, 0.6f, 0.6f);
			glUniform3f(glGetUniformLocation(it->second->Program, "dirLight.color"), 0.8f, .8f, .8f);
		}
	}

	int GetNumShaders()
	{
		return shaders.size();
	}

	/*void SendUniformsToShader(glm::mat4& proj, glm::mat4& mod, glm::mat4& light, glm::vec3& viewPos)
	{
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "lightSpaceMatrix"), 1, GL_FALSE, light[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "CameraViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
		glUniform3f(glGetUniformLocation(sceneShader->Program, "viewPos"), viewCamera->_transform->position.x, viewCamera->_transform->position.y, viewCamera->_transform->position.z);
	}*/

private:
	ShaderManager()
	{
		count = 0;
	}
	~ShaderManager()
	{
		delete instance;
		instance = nullptr;
	}

	std::map<int,Shader*> shaders;

	static ShaderManager* instance;

	int count;
};

#endif