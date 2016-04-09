#version 330 core
layout (location = 0) in vec3 position;
out vec3 texCoords;

uniform mat4 ProjectionMatrix;
uniform mat4 CameraViewMatrix;

void main()
{
	gl_Position = ProjectionMatrix * CameraViewMatrix * vec4(position, 1.0);
	texCoords = position;
}