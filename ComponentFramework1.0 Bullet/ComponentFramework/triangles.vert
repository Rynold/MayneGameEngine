#version 430 core
layout (location = 0) in vec3 vPosition;

uniform mat4 lightSpaceMatrix;
uniform mat4 RotationMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * RotationMatrix * vec4(vPosition, 1.0f);
} 