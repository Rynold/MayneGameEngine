#include "Mesh.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

Mesh::Mesh(EMeshType type)
{
	this->type = type;
	switch (type)
	{
	case EMeshType::CUBE:
	{
		initLightCube();
	}
	break;
	case EMeshType::PLANE:
	{
		initPlane();
	}
	break;
	};
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	glDeleteBuffers(1, &tangentBuffer);
}

void Mesh::initPlane()
{
	GLfloat planeVertices[] = {
		// Positions          // Normals         // Texture Coords
		25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
		-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,

		25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
		-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
		25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f
	};

	std::vector<glm::vec3> tangents;

	for (int i = 0; i < sizeof(planeVertices) / sizeof(GLfloat) - 16; i += 24)
	{
		glm::vec3 vertice(*(planeVertices + i), *(planeVertices + i + 1), *(planeVertices + i + 2));
		glm::vec3 vertice2(*(planeVertices + i + 8), *(planeVertices + i + 9), *(planeVertices + i + 10));
		glm::vec3 vertice3(*(planeVertices + i + 16), *(planeVertices + i + 17), *(planeVertices + i + 18));

		glm::vec2 uv1(*(planeVertices + i + 6), *(planeVertices + i + 7));
		glm::vec2 uv2(*(planeVertices + i + 14), *(planeVertices + i + 15));
		glm::vec2 uv3(*(planeVertices + i + 22), *(planeVertices + i + 23));

		glm::vec3 deltaPos1 = vertice2 - vertice;
		glm::vec3 deltaPos2 = vertice3 - vertice;

		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent;

		tangent.x = r * (deltaUV2.y * deltaPos1.x - deltaUV1.y * deltaPos2.x);
		tangent.y = r * (deltaUV2.y * deltaPos1.y - deltaUV1.y * deltaPos2.y);
		tangent.z = r * (deltaUV2.y * deltaPos1.z - deltaUV1.y * deltaPos2.z);
		tangent = glm::normalize(tangent);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);
	}

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO); //One Buffer to hold the verts, normals, and TexCoords

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), BUFFER_OFFSET(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->tangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->tangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), BUFFER_OFFSET(0));

}

void Mesh::initLightCube()
{
	GLfloat vertices[] = {
		//Back
		// Positions          // Normals           // Texture Coords
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		//front
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,    
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		//left
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //PROBLEM
		-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		//right
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		//bottom
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 
		1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		//top
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  
		1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	//Calculate the tangents and bitangents to pass to the shader
	std::vector<glm::vec3> tangents;

	for (int i = 0; i < sizeof(vertices) / sizeof(GLfloat) - 16; i+=24)
	{
		glm::vec3 vertice(*(vertices + i), *(vertices + i + 1), *(vertices + i + 2));
		glm::vec3 vertice2(*(vertices + i + 8), *(vertices + i + 9), *(vertices + i + 10));
		glm::vec3 vertice3(*(vertices + i + 16), *(vertices + i + 17), *(vertices + i + 18));

		glm::vec2 uv1(*(vertices + i + 6), *(vertices + i + 7));
		glm::vec2 uv2(*(vertices + i + 14), *(vertices + i + 15));
		glm::vec2 uv3(*(vertices + i + 22), *(vertices + i + 23));

		glm::vec3 deltaPos1 = vertice2 - vertice;
		glm::vec3 deltaPos2 = vertice3 - vertice;

		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent;

		tangent.x = r * (deltaUV2.y * deltaPos1.x - deltaUV1.y * deltaPos2.x);
		tangent.y = r * (deltaUV2.y * deltaPos1.y - deltaUV1.y * deltaPos2.y);
		tangent.z = r * (deltaUV2.y * deltaPos1.z - deltaUV1.y * deltaPos2.z);
		tangent = glm::normalize(tangent);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

	}

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO); //One Buffer to hold the verts, normals, and TexCoords

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), BUFFER_OFFSET(6 * sizeof(GLfloat)));
	
	glGenBuffers(1, &this->tangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->tangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), BUFFER_OFFSET(0));

}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Tex> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	material = std::shared_ptr<Material>(new Material());

	type = CUSTOM;
	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	this->setupMesh();
}

void Mesh::setupMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangents));

	glBindVertexArray(0);
}

void Mesh::Draw(std::shared_ptr<Shader> shader)
{
	material->SetupShader(shader);

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Mesh::DrawPlane(std::shared_ptr<Shader> shader)// , GLuint shadowMap, GLuint skybox, bool reflective)
{
	material->SetupShader(shader);// , shadowMap, skybox, reflective);
	
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Mesh::DrawMesh(std::shared_ptr<Shader> shader)
{
	//Find out what type of mesh it is then drawn it accordingly.
	//This can be improved if I use the number of vertices for the
	//mesh. because all the draw calls are practically the same.
	if (type == CUBE)
		DrawCube(shader);
	else if (type == PLANE)
		DrawPlane(shader);
	else if (type == CUSTOM)
		Draw(shader);
}

void Mesh::DrawCube(std::shared_ptr<Shader> shader)
{
	material->SetupShader(shader);
	
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}