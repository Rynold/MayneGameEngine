#include "Scene.h"

using namespace GAME;

Scene::Scene(Window& windowRef): windowPtr(&windowRef) 
{
	drawDebug = false;
}

Scene::~Scene() {
	delete skybox;
	skybox = nullptr;

	TextureManager::GetInstance()->Delete();
	ShaderManager::GetInstance()->Delete();
}

