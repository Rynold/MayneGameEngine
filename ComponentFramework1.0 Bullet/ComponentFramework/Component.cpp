#include "Component.h"
#include "GameObject.h"

Component::Component()
{

}

Component::~Component()
{

}

Component* Component::GetParent()
{
	return parent;
}

void Component::SetParent(Component* p)
{
	parent = p;
}

Component* Component::GetChild()
{
	return child;
}

void Component::SetChild(Component* c)
{
	child = c;
}

GameObject* Component::GetParentObject()
{
	return parentObject;
}

void Component::SetParentObject(GameObject* obj)
{
	parentObject = obj;
}