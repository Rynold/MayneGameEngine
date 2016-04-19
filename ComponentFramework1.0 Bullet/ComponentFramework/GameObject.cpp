#include "GameObject.h"
#include "GLDebugDrawer.h"
#include "Scene0.h"
#include "ModelLoader.h"

GameObject::GameObject()
{
	_transform = new Transform();

	loader = new ModelLoader();
	//scene = scene_;
}

GameObject::~GameObject()
{
	_transform = nullptr;
	delete _transform;

	//Cleans up the components vector
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		delete(*it);
	}
	components.clear();

}

void GameObject::AddRigidBody(RigidBody* body)
{
}

void GameObject::AddComponent(Component* comp)
{
	comp->SetParentObject(this);
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::LoadModel(std::string path)
{
	loader->LoadModel(path, this);
}

void GameObject::Draw(Shader* shader)
{
	///If there isn't anything to draw then just get out of this.
	if (meshs.size() <= 0)
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


		for (int i = 0; i < meshs.size(); ++i){
			meshs[i]->DrawMesh(shader);
		}
	}
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

void GameObject::AttachShader(Shader* shader, const char* name)
{
	shaderID = ShaderManager::GetInstance()->Insert(name, shader);
}