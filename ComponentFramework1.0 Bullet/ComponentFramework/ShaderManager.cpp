#include "ShaderManager.h"

ShaderManager* ShaderManager::instance = 0;

ShaderManager::ShaderManager()
{
	count = 0;
}

void ShaderManager::Delete()
{
	delete instance;
	instance = nullptr;
}

ShaderManager::~ShaderManager()
{
	auto it = shaders.begin();
	while (it != shaders.end())
	{
		it = shaders.erase(it);
	}

	//delete instance;
	//instance = nullptr;
	
}

Shader* ShaderManager::GetShader(int i)
{
	count = 0;
	for (auto it = shaders.begin(); it != shaders.end(); ++it)
	{
		if (count == i)
			return it->second;
		else
			count++;
	}
	std::cerr << "Error:: GetShader() does not contain a shaders for position: " << i << std::endl << "NumShaders: " << GetNumShaders() << std::endl;
}

int ShaderManager::GetNumShaders()
{
	return shaders.size();
}

void ShaderManager::SendLightingDataToShaders()
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

const char* ShaderManager::Insert(const char* id, Shader* shader)
{
	if (!Contains(shader->id)){
		count++;
		shaders.insert(std::pair<const char*, Shader*>(id, shader));
	}

	return id;
}

Shader* ShaderManager::Contains(const char* id)
{
	if (shaders.find(id) != shaders.end())
		return shaders.find(id)->second;
	else
		return nullptr;
}

ShaderManager* ShaderManager::GetInstance()
{
	if (!instance)
		instance = new ShaderManager();

	return instance;
}