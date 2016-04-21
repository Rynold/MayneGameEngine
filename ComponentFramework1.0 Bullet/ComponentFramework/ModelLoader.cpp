#include "ModelLoader.h"

ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::LoadModel(string path, GameObject* object)
{
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// Retrieve the directory path of the filepath
	this->directory = path.substr(0, path.find_last_of('/'));

	// Process ASSIMP's root node recursively
	this->processNode(scene->mRootNode, scene, object);
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene, GameObject* object)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		object->meshs.reserve(node->mNumMeshes);
		object->AttachMesh(processMesh(mesh, scene));
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene, object);
	}
}

Mesh* ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Tex> textures;

	Mesh* thisMesh = new Mesh(EMeshType::CUSTOM);

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		Vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangents = vector;

		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			Vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = Vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace *face = new aiFace();
		face = &(mesh->mFaces[i]);
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face->mNumIndices; j++)
			indices.push_back(face->mIndices[j]);

		face = NULL;
		delete face;
	}
	// Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		Material* mat = new Material();
		aiString str;

		material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		string filename = string(str.C_Str());
		filename = directory + '/' + filename;
		mat->AttachTexture(filename.c_str(), TextureType::DIFFUSE);

		material->GetTexture(aiTextureType_SPECULAR, 0, &str);
		filename = string(str.C_Str());
		filename = directory + '/' + filename;
		mat->AttachTexture(filename.c_str(), TextureType::SPECULAR);

		material->GetTexture(aiTextureType_HEIGHT, 0, &str);
		filename = string(str.C_Str());
		filename = directory + '/' + filename;
		//mat->AttachTexture(filename.c_str(), TextureType::NORMAL);

		thisMesh = new Mesh(vertices, indices, textures);
		thisMesh->material = mat;
	}

	// Return a mesh object created from the extracted mesh data
	return thisMesh;
}