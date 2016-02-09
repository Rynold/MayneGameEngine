#include "GameObject.h"


GameObject::GameObject()
{
	_transform = new Transform();
	//_staticMesh = new Mesh(EMeshType::CUBE);*/
	//scene = scene_;
}

GameObject::~GameObject()
{
	_transform = nullptr;
	delete _transform;

	_staticMesh = nullptr;
	delete _staticMesh;

	//Cleans up the components vector
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		delete(*it);
	}
	components.clear();

}

void GameObject::AddComponent(Component* comp)
{
	/*if ((Transform*)comp)
		_transform = (Transform*)comp;*/
	if ((Mesh*)comp)
		_staticMesh = (Mesh*)comp;

	//comp->owner = this;
}

void GameObject::Update(float deltaTime)
{
	//_transform->position += Vec3(0.0,0.01,0.01);
	//_transform->rotation.w += 0.01;
	//std::cout << "Update: " << _transform->position.x << _transform->position.y << _transform->position.z << std::endl;
}

void GameObject::Draw(Shader* shader, GLuint shadowMap)
{
	///If there isn't anything to draw then just get out of this.
	if (_staticMesh == nullptr)
		return;

	shader->Use();

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(_transform->position.x, _transform->position.y, _transform->position.z));
	glm::mat4 scale = glm::scale(translation, glm::vec3(_transform->scale.x, _transform->scale.y, _transform->scale.z));
	glm::mat4 rotate = glm::rotate(scale, _transform->rotation.w, glm::vec3(_transform->rotation.x, _transform->rotation.y, _transform->rotation.z));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "RotationMatrix"), 1, GL_FALSE, &rotate[0][0]);


	_staticMesh->DrawMesh(shader, shadowMap);
}