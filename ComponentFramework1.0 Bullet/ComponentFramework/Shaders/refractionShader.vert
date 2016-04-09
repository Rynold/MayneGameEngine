#version 400 core
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 normal;

uniform mat4 RotationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 CameraViewMatrix;

out vec3 Normal;
out vec3 Position;

void main()
{
	gl_Position = ProjectionMatrix * CameraViewMatrix * RotationMatrix * vPosition;
	Position = vec3(RotationMatrix * vPosition);
	Normal = (transpose(inverse(mat3(RotationMatrix)))) * normal;
}