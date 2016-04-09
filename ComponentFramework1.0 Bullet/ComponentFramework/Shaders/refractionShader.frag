#version 430 core

in vec3 Normal;
in vec3 Position;
out vec4 fColor;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(Position - viewPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	fColor = texture(skybox, R);
}