#include "Scene.h"

using namespace GAME;

Scene::Scene(Window& windowRef): windowPtr(&windowRef) 
{
}

Scene::~Scene() {
	delete skybox;
	skybox = nullptr;
}

