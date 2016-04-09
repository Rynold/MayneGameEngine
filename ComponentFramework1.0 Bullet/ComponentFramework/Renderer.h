#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.h"

class Renderer
{
public:
	static Renderer* GetInstance()
	{
		if (!instance)
			instance = new Renderer();

		return instance;
	}

	//void RenderScene(Scene* scene);

private:
	Renderer(){}
	~Renderer(){}

	static Renderer* instance;
};

#endif