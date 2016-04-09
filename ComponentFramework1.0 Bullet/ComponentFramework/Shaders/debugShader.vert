#version 430 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out vec4 in_color;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 RotationMat;

void main()
{
	gl_Position = ProjectionMatrix * ModelViewMatrix *  position;
	in_color = color;
}