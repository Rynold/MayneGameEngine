#version 400 core
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 tangent;

uniform mat4 RotationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 CameraViewMatrix;
uniform mat4 lightSpaceMatrix;

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out mat3 TBN;

void main()
{	
	//Color = VertexColor;
	texCoord = vTexCoord;
	Normal = mat3(transpose(inverse(RotationMatrix))) * normal;
	FragPos = vec3(RotationMatrix * vPosition);
	gl_Position = ProjectionMatrix * CameraViewMatrix * RotationMatrix * vPosition;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	mat3 normalMatrix = transpose(inverse(mat3(RotationMatrix)));
	vec3 T = normalize(normalMatrix * tangent);
	vec3 N = normalize(normalMatrix * normal);
	
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T,N);

	TBN = transpose(mat3(T,B,N));
}

//G25X25S5   bell confirm #