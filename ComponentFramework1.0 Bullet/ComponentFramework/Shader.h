#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h> // Include glew to get all the required OpenGL headers

class Shader
{
public:
	// The program ID
	GLuint Program;
	
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	Shader(){}
	~Shader();
	// Use the program
	void Use();

	//float Noise(glm::vec3 x);

	const char* id;
};

#endif