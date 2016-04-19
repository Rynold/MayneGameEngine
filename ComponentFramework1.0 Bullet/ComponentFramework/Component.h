#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "Transform.h"

class GameObject;

class Component
{
public:
	Component();
	~Component();

	//Transform _transform;

	virtual void Update() = 0;

	void SetParent(Component* p);

	Component* GetParent();

	void SetChild(Component* c);

	Component* GetChild();

	void SetParentObject(GameObject* obj);

	GameObject* GetParentObject();

private:

	Component* parent;
	Component* child;

	GameObject* parentObject;
};

#endif