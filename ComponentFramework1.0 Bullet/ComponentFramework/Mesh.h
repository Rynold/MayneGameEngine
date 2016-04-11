#pragma once
#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Component.h"
using namespace std;

#include <GL/glew.h> // Contains all the necessery OpenGL includes
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "IOStream.hpp"

#include "Shader.h"
#include "Vector.h"
#include "Material.h"
#include "TextureManager.h"
#include "ShaderManager.h"

using namespace MATH;

enum EMeshType
{
	CUBE,
	SPHERE,
	PLANE,
	MESH,
	CUSTOM
};

struct Vertex {
	Vec3 Position;
	Vec3 Normal;
	Vec2 TexCoords;
	Vec3 Tangents;
};

struct Tex {
	GLuint id;
	string type;
	aiString path;
};

class Mesh : public Component {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Tex> textures;

	Material* material;

	/*  Functions  */
	Mesh(EMeshType type);
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Tex> textures);
	~Mesh();
	void Draw(Shader* shader);
	void Draw(Shader* shader, int temp);
	void DrawPlane(Shader* shader);// , GLuint shadowMap, GLuint skybox, bool reflective);
	void DrawCube(Shader* shader);// , GLuint shadowMap, GLuint skybox, bool reflective);
	void DrawMesh(Shader* shader);// , GLuint shadowMap, GLuint skybox, bool reflective);

	EMeshType type;


private:
	/*  Render data  */
	GLuint VAO, VBO, EBO, tangentBuffer;

	void initCube();
	void initLightCube();
	void initPlane();
	/*  Functions    */
	void setupMesh();
	void setupMyMesh();
};

#endif;