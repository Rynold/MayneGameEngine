#include "Mesh.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

Mesh::Mesh(EMeshType type)
{
	switch (type)
	{
	case EMeshType::CUBE:
	{
		initLightCube();
		this->type = type;
	}
	break;
	case EMeshType::PLANE:
	{
		initPlane();
		this->type = type;
	}
	break;
	};

	//material = new Material("Images/Parallax/wood.png", "Images/container_specular.png", "Images/Parallax/toy_box_normal.png", "Images/Parallax/bricks_normal.jpg", "Images/Parallax/toy_box_disp.png");
	//material = new Material("Images/BrickWall/brickwall.jpg", "Images/container_specular.png", "Images/BrickWall/brickwall_normal.jpg");
	//material = new Material("Images/RyanBrickTextures/wall_BaseColor.png", "Images/RyanBrickTextures/wall_Roughness.png", "Images/RyanBrickTextures/wall_Normal.png", "Images/RyanBrickTextures/wall_Roughness.png", "Images/RyanBrickTextures/wall_height.png");
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

void Mesh::initCube()
{
	Vertex vert;
	vert.Position = Vec3(-1.0, -1.0, 1.0);   // Front Bottom left / 0
	vert.Normal = Vec3(0.0, 0.0, 1.0);
	vert.TexCoords = Vec2(0.0, 1.0);
	vertices.push_back(vert);

	vert.Position = Vec3(1.0, -1.0, 1.0); // Front Bottom right / 1
	vert.Normal = Vec3(0.0, 0.0, 1.0);
	vert.TexCoords = Vec2(1.0, 1.0);
	vertices.push_back(vert);

	vert.Position = Vec3(1.0, 1.0, 1.0); // Front Top Right / 2
	vert.Normal = Vec3(0.0, 0.0, 1.0);
	vert.TexCoords = Vec2(1.0, 0.0);
	vertices.push_back(vert);

	vert.Position = Vec3(-1.0, 1.0, 1.0); // Front Top Left / 3
	vert.Normal = Vec3(0.0, 0.0, 1.0);
	vert.TexCoords = Vec2(0.0, 0.0);
	vertices.push_back(vert);

	vert.Position = Vec3(-1.0, -1.0, -1.0); // Back Botton Left / 4
	vert.Normal = Vec3(-1.0, 0.0, 0.0);
	vert.TexCoords = Vec2(0.0, 1.0);
	vertices.push_back(vert);

	vert.Position = Vec3(-1.0, 1.0, -1.0); // Back Top Left / 5
	vert.Normal = Vec3(-1.0, 0.0, 0.0);
	vert.TexCoords = Vec2(1.0, 1.0);
	vertices.push_back(vert);

	vert.Position = Vec3(1.0, 1.0, -1.0); // Back Top Right / 6
	vert.Normal = Vec3(-1.0, 0.0, 0.0);
	vert.TexCoords = Vec2(1.0, 0.0);
	vertices.push_back(vert);

	vert.Position = Vec3(1.0, -1.0, -1.0); // Back Bottom Right / 7
	vert.Normal = Vec3(-1.0, 0.0, 0.0);
	vert.TexCoords = Vec2(0.0, 0.0);
	vertices.push_back(vert);

	// Front face
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// Back Face
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(7);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	// Top Face
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(5);

	indices.push_back(2);
	indices.push_back(5);
	indices.push_back(6);

	//Bottom Face
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(4);

	indices.push_back(1);
	indices.push_back(4);
	indices.push_back(7);

	//Right Face
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(6);

	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(7);

	//Left Face
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(4);

	indices.push_back(3);
	indices.push_back(5);
	indices.push_back(4);

	/*Texture texture;
	texture.id = textureID;
	texture.path = path;*/

	//textures.insert(textures.end(), texture);

	material = new Material();

	this->setupMesh();
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

	material = new Material();

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

void Mesh::Draw(Shader* shader)
{

	material->SetupShader(shader);
	//// Bind appropriate textures
	//GLuint diffuseNr = 1;
	//GLuint specularNr = 1;
	//for (GLuint i = 0; i < this->textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
	//	// Retrieve texture number (the N in diffuse_textureN)
	//	stringstream ss;
	//	string number;
	//	string name = this->textures[i].type;
	//	if (name == "diffuse")
	//		ss << diffuseNr++; // Transfer GLuint to stream
	//	else if (name == "specular")
	//		ss << specularNr++; // Transfer GLuint to stream
	//	number = ss.str();
	//	// Now set the sampler to the correct texture unit
	//	glUniform1i(glGetUniformLocation(shader->Program, ("material." + name).c_str()), i);
	//	// And finally bind the texture
	//	glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	//}

	//// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
	//glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), 16.0f);

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::DrawPlane(Shader* shader)// , GLuint shadowMap, GLuint skybox, bool reflective)
{
	material->SetupShader(shader);// , shadowMap, skybox, reflective);
	
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Mesh::DrawMesh(Shader* shader)//, GLuint shadowMap, GLuint skybox, bool reflective)
{
	if (type == CUBE)
		DrawCube(shader);// , shadowMap, skybox, reflective);
	else if (type == PLANE)
		DrawPlane(shader);// , shadowMap, skybox, reflective);
}

void Mesh::DrawCube(Shader* shader)//, GLuint shadowMap, GLuint skybox, bool reflective)
{
	material->SetupShader(shader);// , shadowMap, skybox, reflective);
	
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}