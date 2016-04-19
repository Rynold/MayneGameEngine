#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include "Texture.h"
#include <GL/glew.h>

class TextureManager
{
public:

	static TextureManager* GetInstance();

	Texture* Contains(const char* name);

	void Insert(const char* path, Texture* tex);

	void LoadTexture(const char* path, TextureType type);

private:
	TextureManager();
	~TextureManager();

	static TextureManager* manager;

	std::map<const char*, Texture*> textures;

};

#endif