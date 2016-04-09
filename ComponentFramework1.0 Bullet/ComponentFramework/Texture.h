#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "SOIL.h"
#include <GL/glew.h>
#include <iostream>

enum TextureType
{
	DIFFUSE,
	SPECULAR,
	NORMAL,
	ROUGHNESS,
	HEIGHT,
	REFLECTION
};


class Texture
{
public:

	Texture(const char* name, TextureType type, GLuint tex)
	{
		this->name = name;
		this->type = type;
		this->tex = tex;
		active = true;
	}
	~Texture(){}

	TextureType type;
	bool active;
	GLuint tex;
	const char* name;

private:

};

#endif