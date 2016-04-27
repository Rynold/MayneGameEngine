#version 400 core
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 tangent;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 CameraViewMatrix;
uniform mat4 lightSpaceMatrix;

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out mat3 TBN;
out vec3 Position;

void main()
{	
	//Color = VertexColor;
	texCoord = vTexCoord;
	
	// When you get the inverse transpose of the RotationMatrix it will return a matrix
	// That only has the rotation.
	mat3 normalMatrix = transpose(inverse(mat3(ModelViewMatrix)));
	Normal = normalMatrix * normal;
	Position = vec3(ModelViewMatrix * vPosition);
	
	FragPos = vec3(ModelViewMatrix * vPosition);
	gl_Position = ProjectionMatrix * CameraViewMatrix * ModelViewMatrix * vPosition;
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	//Transform the vectors to the coordinate system we'd like to work in.
	vec3 T = normalize(normalMatrix * tangent);
	vec3 N = normalize(normalMatrix * normal);

	// Calculate the bitangent using the cross of the tangent and normal
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T,N);

	TBN = transpose(mat3(T,B,N));
	
	// Tangent space is a space thats local to the surface of a triangle. Because a normal map has its
	// vecters always pointing in the same direction you have to convert everything to be relative to
	// the face of the triangle you're currently working with.
}

//G25X25S5   bell confirm #