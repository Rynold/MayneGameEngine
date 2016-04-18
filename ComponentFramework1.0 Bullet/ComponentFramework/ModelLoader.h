#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "GameObject.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <SOIL.h>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include "Material.h"
#include "Mesh.h"

class ModelLoader
{
public:

	ModelLoader();
	~ModelLoader();

	string directory;

	void LoadModel(string path, GameObject* object);

private:

	void processNode(aiNode* node, const aiScene* scene, GameObject* object);

	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};

#endif