#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include "Texture.h"
#include <GL/glew.h>

class TextureManager
{
public:

	static TextureManager* GetInstance()
	{
		if (!manager)
			manager = new TextureManager();

		return manager;
	}

	Texture* Contains(const char* name)
	{
		if (textures.find(name) != textures.end())
			return textures.find(name)->second;
		else
			return nullptr;
	}

	void Insert(const char* path, Texture* tex)
	{
		textures.insert(std::pair<const char*, Texture*>(path,tex));
	}

	void LoadTexture(const char* path, TextureType type)
	{
		GLuint texture;
		if (!TextureManager::GetInstance()->Contains(path))
		{
			glGenTextures(1, &texture);

			int width, height;
			unsigned char* image;

			image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
			if (0 == image)
			{
				std::cout << "SOIL loading error: \n" << SOIL_last_result() << std::endl;
			}
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

			TextureManager::GetInstance()->Insert(path, new Texture(path, type, texture));
		}
	}

private:
	TextureManager(){
		//this->LoadTexture("Images/defaultNormal.jpg", TextureType::NORMAL);
	}
	~TextureManager()
	{
		delete manager;
		manager = nullptr;
	}

	static TextureManager* manager;

	std::map<const char*, Texture*> textures;

};

#endif