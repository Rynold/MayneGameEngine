#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include "Texture.h"
#include <GL/glew.h>
#include <memory>

class TextureManager
{
public:

	static TextureManager* GetInstance();

	std::shared_ptr<Texture> Contains(const char* name);

	void Insert(const char* path, std::shared_ptr<Texture> tex);

	void LoadTexture(const char* path, TextureType type);

	~TextureManager();

	void Delete();

private:
	TextureManager();
	

	static TextureManager* manager;

	std::map<const char*,std::shared_ptr<Texture>> textures;

};

#endif