#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "MColor.h"
#include "SOIL.h"

class Material
{
public:
	MColor base;
	MColor ambient;
	MColor diffuse;
	MColor specular;
	float roughness;

	GLuint baseTexture;
	GLuint specularTexture;
	GLuint normalMap;
	GLuint roughnessMap;
	GLuint heightMap;

	Material()
	{
		base = MColor::WHITE();
		specular = MColor::WHITE();
		ambient = MColor::RED();
		diffuse = MColor::RED();
		roughness = 32.0f;
	}

	Material(const char* path)
	{
		base = MColor::WHITE();
		specular = MColor::WHITE();
		ambient = MColor::RED();
		diffuse = MColor::RED();
		roughness = 32.0f;

		LoadTexture(path, baseTexture);
	}

	Material(const char* diffusePath, const char* specularPath)
	{
		base = MColor::WHITE();
		specular = MColor::WHITE();
		ambient = MColor::RED();
		diffuse = MColor::RED();
		roughness = 32.0f;

		LoadTexture(diffusePath, baseTexture);
		LoadTexture(specularPath, specularTexture);
	}

	Material(const char* diffusePath, const char* specularPath, const char* normalPath)
	{
		base = MColor::WHITE();
		specular = MColor::WHITE();
		ambient = MColor::RED();
		diffuse = MColor::RED();
		roughness = 64.0f;

		LoadTexture(diffusePath, baseTexture);
		LoadTexture(specularPath, specularTexture);
		LoadTexture(normalPath, normalMap);
		
	}

	Material(const char* diffusePath, const char* specularPath, const char* normalPath, const char* roughnessPath)
	{
		base = MColor::WHITE();
		specular = MColor::WHITE();
		ambient = MColor::RED();
		diffuse = MColor::RED();
		roughness = 64.0f;

		LoadTexture(diffusePath, baseTexture);
		LoadTexture(specularPath, specularTexture);
		LoadTexture(normalPath, normalMap);
		LoadTexture(roughnessPath, roughnessMap);
	}

	Material(const char* diffusePath, const char* specularPath, const char* normalPath, const char* roughnessPath, const char* heightPath)
	{
		base = MColor::WHITE();
		specular = MColor::WHITE();
		ambient = MColor::RED();
		diffuse = MColor::RED();
		roughness = 64.0f;

		LoadTexture(diffusePath, baseTexture);
		LoadTexture(specularPath, specularTexture);
		LoadTexture(normalPath, normalMap);
		LoadTexture(roughnessPath, roughnessMap);
		LoadTexture(heightPath, heightMap);
	}

	~Material();

	void LoadTexture(const char* path, GLuint &location)
	{
		glGenTextures(1, &location);

		int width, height;
		unsigned char* image;

		image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
		if (0 == image)
		{
			std::cout << "SOIL loading error: \n" << SOIL_last_result() << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, location);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}

};

#endif