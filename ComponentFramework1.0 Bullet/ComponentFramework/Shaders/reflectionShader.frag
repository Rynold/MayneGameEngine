#version 430 core

in vec3 Normal;
in vec3 Position;
out vec4 fColor;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{
	vec3 I = normalize(Position - viewPos);
	vec3 R = reflect(I, normalize(Normal));
	fColor = texture(skybox, R);
}