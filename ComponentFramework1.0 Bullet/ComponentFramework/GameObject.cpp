#include "GameObject.h"
#include "GLDebugDrawer.h"
#include "Scene0.h"

GameObject::GameObject()
{
	_transform = new Transform();
	reflective = false;
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

void GameObject::AddRigidBody(RigidBody* body)
{
	//rigidBody = body;

	
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
	/*_transform->position.x = rigidBody->getTransform().getTranslation().getComponent<0>();
	_transform->position.y = rigidBody->getTransform().getTranslation().getComponent<1>();
	_transform->position.z = rigidBody->getTransform().getTranslation().getComponent<2>();

	_transform->rotationHK = rigidBody->getRotation();*/
}

void GameObject::Draw(Shader* shader)// , GLuint shadowMap, GLuint skybox, bool reflective)
{
	///If there isn't anything to draw then just get out of this.
	if (_staticMesh == nullptr && model == nullptr)
		return;
	else{

		shader->Use();


		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(_transform->Scale().x, _transform->Scale().y, _transform->Scale().z));
		glm::mat4 rotate = glm::rotate(scale, _transform->Rotation().w, glm::vec3(_transform->Rotation().x, _transform->Rotation().y, _transform->Rotation().z));
		glm::mat4 translation = glm::translate(rotate, glm::vec3(_transform->Position().x, _transform->Position().y, _transform->Position().z));
		//glUniformMatrix4fv(glGetUniformLocation(shader->Program, "RotationMatrix"), 1, GL_FALSE, &translation[0][0]);
		GLfloat mat[16];
		_transform->GetBulletTransform().getOpenGLMatrix(&mat[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "RotationMatrix"), 1, GL_FALSE, &mat[0]);
	}

	if (_staticMesh != nullptr)
		_staticMesh->DrawMesh(shader);

	if (model != nullptr)
		model->Draw(shader);
}

void GameObject::DrawDebug(btDynamicsWorld* world)
{
	if (body != nullptr)
		world->debugDrawObject(body->getWorldTransform(),body->getCollisionShape(), btVector3(1.0,0.0,0.0));
}

Shader* GameObject::GetShader()
{
	return ShaderManager::GetInstance()->Contains(shaderID);
}

void GameObject::AttachShader(Shader* shader)
{
	shaderID = ShaderManager::GetInstance()->Insert(-1, shader);
}