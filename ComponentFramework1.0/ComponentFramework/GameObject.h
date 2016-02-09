#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Transform.h"
#include "Mesh.h"
#include "Component.h"
#include "MathUtil.h"
#include "glm.hpp"
#include "gtc\matrix_transform.inl"

using namespace MATH;

class GameObject
{
public:
	GameObject();
	~GameObject();

	GameObject(const GameObject&) = delete;
	GameObject(GameObject&&) = delete;
	GameObject& operator = (const GameObject&) = delete;
	GameObject& operator = (GameObject&&) = delete;

	virtual void Update(float deltaTime);

	Transform* _transform;
	Mesh* _staticMesh;

	/* Will add a specific component to this game object
	   @param1 the component to attach.*/
	void AddComponent(Component* comp);

	/* 
	* Will remove a specific component to this GameObject
	* @param1, the component to removed 
	*/
	void RemoveComponent(Component* comp);

	/*
	 * Gets a reference to a component attached to this object.
	 * @param The type of component you are looking for.
	 * @return A reference to that component, NULL if there is none. 
	 */
	Component* GetReferenceToComponent(Component* comp);

	/*
	* Gets a reference to all the components attached to this object.
	* 
	* @return A vector of all components attached to this object.
	*/
	std::vector<Component*> GetAllComponents();

	//inline Scene* getScene() { return scene; };

	void Draw(Shader* shader, GLuint shadowMap);

private:

	std::vector<Component*> components;

	//Scene* scene;
};

#endif