#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "MColor.h"
#include "SOIL.h"
#include "Texture.h"
#include "ShaderManager.h"
#include <iostream>
#include <stdint.h>
#include "TextureManager.h"

class Material
{
private:
	const char* baseTexture;
	const char* specularTexture;
	const char* normalMap;
	const char* roughnessMap;
	const char* heightMap;
	const char* reflectionMap;

	uint32_t baseFlag;
	uint32_t specFlag;
	uint32_t normFlag;
	uint32_t roughFlag;
	uint32_t heightFlag;
	uint32_t reflFlag;

	uint32_t matFlags;
	

public:
	MColor base;
	MColor ambient;
	MColor diffuse;
	MColor specular;
	float roughness;
	const char* shaderID;
	

	Material()
	{
		base = MColor::WHITE();
		specular = MColor::WHITE();
		ambient = MColor::RED();
		diffuse = MColor::RED();
		roughness = 32.0f;

		matFlags = 0x00000000;

		baseFlag   = 0x00000001;
		specFlag   = 0x00000010;
		normFlag   = 0x00000100;
		roughFlag  = 0x00001000;
		heightFlag = 0x00010000;
		reflFlag   = 0x00100000;
	}

	void PrintMatFlags()
	{
		std::cout << matFlags << std::endl;
	}

	~Material()
	{

	}

	void AttachShader(std::shared_ptr<Shader> shader, const char* name)
	{
		shaderID = ShaderManager::GetInstance()->Insert(name, shader);
	}

	void AttachTexture(const char* path, TextureType type)
	{
		switch (type)
		{
		case TextureType::DIFFUSE:
			baseTexture = path;
			matFlags += baseFlag;
			break;
		case TextureType::SPECULAR:
			specularTexture = path;
			matFlags += specFlag;
			break;
		case TextureType::ROUGHNESS:
			roughnessMap = path;
			matFlags += roughFlag;
			break;
		case TextureType::NORMAL:
			normalMap = path;
			matFlags += normFlag;
			break;
		case TextureType::HEIGHT:
			heightMap = path;
			matFlags += heightFlag;
			break;
		case TextureType::REFLECTION:
			reflectionMap = path;
			matFlags += reflFlag;
			break;
		}

		//PrintMatFlags();

		TextureManager::GetInstance()->LoadTexture(path, type);
	}

	void SetupShader(std::shared_ptr<Shader> shader)
	{

		TextureManager* texMan = TextureManager::GetInstance();
		if (matFlags & baseFlag){
			if (texMan->Contains(baseTexture))
			{
				glUniform1i(glGetUniformLocation(shader->Program, "material.baseTex"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texMan->Contains(baseTexture)->tex);
			}
		}
		else{
			glUniform1i(glGetUniformLocation(shader->Program, "material.baseTex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,0);
		}
		if (matFlags & specFlag){
			if (texMan->Contains(specularTexture))
			{
				glUniform1i(glGetUniformLocation(shader->Program, "material.specular"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, texMan->Contains(specularTexture)->tex);
			}
		}

		if (matFlags & normFlag){
			if (texMan->Contains(normalMap))
			{
				glUniform1i(glGetUniformLocation(shader->Program, "material.normalMap"), 2);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, texMan->Contains(normalMap)->tex);
			}
		}
		else{
			glUniform1i(glGetUniformLocation(shader->Program, "material.normalMap"), 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texMan->Contains("Images/defaultNormal.png")->tex);
		}
		if (matFlags & roughFlag){
			if (texMan->Contains(roughnessMap))
			{
				glUniform1i(glGetUniformLocation(shader->Program, "material.roughnessMap"), 3);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, texMan->Contains(roughnessMap)->tex);
			}
		}

		if (matFlags & heightFlag){
			if (texMan->Contains(heightMap))
			{
				glUniform1i(glGetUniformLocation(shader->Program, "material.depthMap"), 4);
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, texMan->Contains(heightMap)->tex);
			}
		}
		glUniform1ui(glGetUniformLocation(shader->Program, "material.matFlags"), matFlags);

		glUniform1f(glGetUniformLocation(shader->Program, "material.roughness"), 32);

		texMan = nullptr;
	}
};

#endif