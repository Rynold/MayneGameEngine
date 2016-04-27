#version 400 core
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vTexCoord;

out vec2 TexCoords;
out vec3 fragPosition;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 CameraViewMatrix;

void main()
{
    gl_Position = ProjectionMatrix * CameraViewMatrix * ModelViewMatrix * vPosition;
    fragPosition = vec3(ModelViewMatrix * vPosition);
    Normal = mat3(transpose(inverse(ModelViewMatrix))) * normal;
    TexCoords = vTexCoord;
}
