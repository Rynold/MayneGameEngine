#version 430 core
layout (location = 0) in vec3 vPosition;

uniform mat4 lightSpaceMatrix;
uniform mat4 ModelViewMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * ModelViewMatrix * vec4(vPosition, 1.0f);
} 