#version 400 core
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 RotationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 CameraViewMatrix;
uniform mat4 lightSpaceMatrix;

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

void main()
{	
	//Color = VertexColor;
	texCoord = vTexCoord;
	Normal = mat3(transpose(inverse(RotationMatrix))) * normal;
	FragPos = vec3(RotationMatrix * vPosition);
	gl_Position = ProjectionMatrix * CameraViewMatrix * RotationMatrix * vPosition;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
