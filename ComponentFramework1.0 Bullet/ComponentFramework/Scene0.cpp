#include "GL/glew.h"
#include <SDL.h>
#include "Scene0.h"
#include "gtc\type_ptr.hpp"
#include "MyMotionState.h"
#include "Uniforms.h"
#include "Model.h"
#include <iostream>
using namespace GAME;

void RenderQuad();

float angle = 45;

Scene0::Scene0(class Window& windowRef):  Scene(windowRef) {
	viewCamera = new Camera(glm::vec3(0.0, 4.0, 10.0), glm::vec3(0.0, 1.0, 0.0), -90, -20);
	temp = new Shader("Shaders/TBNShader.vert", "Shaders/TBNShader.frag");
	sceneShader = new Shader("Shaders/ParallaxShader.vert", "Shaders/ParallaxShader.frag");
	skyboxShader = new Shader("Shaders/skybox.vert", "Shaders/skybox.frag");
	reflectiveShader = new Shader("Shaders/reflectionShader.vert", "Shaders/reflectionShader.frag");
	sceneShader->Use();
	pointLight = new PointLight();
	dirLight = new DirectionalLight();
	skybox = new Skybox();
	bP = new BulletPhysics();

	firstMouse = true;
	rightClickDown = false;

	char playerChoice[10];
	bool properChoice = false;

	while (!properChoice){
		

		std::cout << "Start as Server? (Y/N):" << std::endl;
		std::cin >> playerChoice;
		//playerChoice[0] = 'Y';

		if (playerChoice[0] == 'Y' || playerChoice[0] == 'y'){
			nM = new NetworkManager(true);
			properChoice = true;
		}
		else if (playerChoice[0] == 'N' || playerChoice[0] == 'n'){
			nM = new NetworkManager(false);
			properChoice = true;
		}
		else
		{
			std::cout << "Please select either 'Y' or 'N'" << std::endl;
		}
	}

	TextureManager::GetInstance()->LoadTexture("Images/defaultNormal.png", TextureType::NORMAL);

}

Scene0::~Scene0(){ 
	delete pointLight;
	pointLight = nullptr;

	delete dirLight;
	dirLight = nullptr;

	delete reflectiveShader;
	reflectiveShader = nullptr;

	glDeleteFramebuffers(1, &this->depthMapFBO);
	// Skybox memory is freed from parent class
}

bool Scene0::OnCreate() {
	OnResize(windowPtr->GetWidth(), windowPtr->GetHeight());
	/// Load Assets: as needed 
	sceneShader->Use();

	Material* boxMat = new Material();
	boxMat->AttachTexture("Images/Parallax/wood.png", TextureType::DIFFUSE);
	boxMat->AttachTexture("Images/Parallax/toy_box_normal.png", TextureType::NORMAL);
	boxMat->AttachTexture("Images/Parallax/toy_box_disp.png", TextureType::HEIGHT);

	Material* floorMat = new Material();
	floorMat->AttachTexture("Images/BrickWall/brickwall.jpg", TextureType::DIFFUSE);
	floorMat->AttachTexture("Images/BrickWall/brickwall_normal.jpg", TextureType::NORMAL);

	Material* brickMat = new Material();
	brickMat->AttachTexture("Images/RyanBrickTextures/wall_BaseColor.png", TextureType::DIFFUSE);
	brickMat->AttachTexture("Images/RyanBrickTextures/wall_Normal.png", TextureType::NORMAL);
	brickMat->AttachTexture("Images/RyanBrickTextures/wall_height.png", TextureType::HEIGHT);
	brickMat->AttachTexture("Images/RyanBrickTextures/wall_Roughness.png", TextureType::SPECULAR);

	// Clean up this spahgetti and put it in the RigidBody class
	{
		floor_ = new GameObject();
		floor_->_transform->SetPosition(0.0, -5.0, 0.0);
		//floor_->_transform->SetRotation(0, 0, 1, 0);
		floor_->AttachMesh(new Mesh(EMeshType::PLANE));
		floor_->AttachShader(temp, "temp");
		floor_->GetMesh()->material = floorMat;

		float mass = 0;
		btCollisionShape* floorShape = new btStaticPlaneShape(btVector3(0,1,0), -0.5);
		btVector3 localInertia(0, 0, 0);
		MyMotionState* boxMotionState = new MyMotionState(floor_->_transform->GetBulletTransform(),floor_);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, boxMotionState, floorShape, localInertia);
		btRigidBody* floorBody = new btRigidBody(rbInfo);
		floorBody->setWorldTransform(floor_->_transform->GetBulletTransform());
		bP->m_dynamicsWorld->addRigidBody(floorBody);
		bP->m_collisionShapes.push_back(floorShape);

		floor_->body = floorBody;

		GameObjects.push_back(floor_);
	}

	{
		box = new GameObject();
		box->AttachMesh(new Mesh(EMeshType::CUBE));
		box->GetMesh()->material = boxMat;
		box->AttachShader(sceneShader, "sceneShader");

		box->_transform->SetPosition(0, 0, 0);
		box->_transform->SetAxisAngleDeg(1,0,1,45);

		float mass = 5;
		btCollisionShape* boxShape = new btBoxShape(btVector3(1,1,1));
		btVector3 localInertia(0, 0, 0);
		boxShape->calculateLocalInertia(mass, localInertia);

		MyMotionState* boxMotionState = new MyMotionState(box->_transform->GetBulletTransform(), box);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, boxMotionState, boxShape, localInertia);
		btRigidBody* boxBody = new btRigidBody(rbInfo);
		boxBody->setWorldTransform(box->_transform->GetBulletTransform());
		bP->m_dynamicsWorld->addRigidBody(boxBody);
		bP->m_collisionShapes.push_back(boxShape);

		box->body = boxBody;

		GameObjects.push_back(box);
	}

	for (int i = 0; i < 5; i++)
	{
		GameObject* cube = new GameObject();
		cube->AttachMesh(new Mesh(EMeshType::CUBE));
		cube->GetMesh()->material = boxMat;
		cube->AttachShader(sceneShader, "sceneShader");

		cube->_transform->SetPosition(0, 2.5 * i, 0);
		//cube->_transform->SetRotation(1, 0, 1, 45);

		float mass = 5;
		btCollisionShape* boxShape = new btBoxShape(btVector3(1, 1, 1));
		btVector3 localInertia(0, 0, 0);
		boxShape->calculateLocalInertia(mass, localInertia);

		MyMotionState* boxMotionState = new MyMotionState(cube->_transform->GetBulletTransform(), cube);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, boxMotionState, boxShape, localInertia);
		btRigidBody* boxBody = new btRigidBody(rbInfo);
		boxBody->setWorldTransform(cube->_transform->GetBulletTransform());
		bP->m_dynamicsWorld->addRigidBody(boxBody);
		bP->m_collisionShapes.push_back(boxShape);

		cube->body = boxBody;

		GameObjects.push_back(cube);
	}

	reflectiveBox = new GameObject();
	reflectiveBox->AttachMesh(new Mesh(EMeshType::CUBE));
	reflectiveBox->GetMesh()->material = brickMat;
	reflectiveBox->AttachShader(reflectiveShader, "reflectiveShader");

	reflectiveBox->_transform->SetPosition(-3, 0.0, 0.0);
	GameObjects.push_back(reflectiveBox);

	GameObject* refractionBox = new GameObject;
	refractionBox->AttachMesh(new Mesh(EMeshType::CUBE));
	refractionBox->GetMesh()->material = brickMat;
	refractionBox->AttachShader(new Shader("Shaders/refractionShader.vert","Shaders/refractionShader.frag"), "refractionShader");
	refractionBox->_transform->SetPosition(-6, 0.0, 0.0);
	GameObjects.push_back(refractionBox);

	GameObject* nanoSuit = new GameObject;
	nanoSuit->LoadModel("Models/Barrel/RyanBarrel/barrel.obj");
	nanoSuit->_transform->SetPosition(0.0, -5.0, -5.0);
	nanoSuit->_transform->SetAxisAngleDeg(0,1,0,0);
	nanoSuit->_transform->SetScale(0.3, 0.3, 0.3);
	nanoSuit->AttachShader(new Shader("Shaders/modelShader.vert","Shaders/modelShader.frag"), "modelShader");
	GameObjects.push_back(nanoSuit);

	nM->AttachListener(viewCamera);

	pointLight->_transform->SetPosition(0.0, 0.0, 2.0);
	dirLight->_transform->SetPosition(-4.0, 8.0, -2.0);

	sceneShader->Use();

	ShaderManager::GetInstance()->SendLightingDataToShaders();

	dirLight->GenerateDepthMapFBO();

	return true;
}

void Scene0::OnDestroy(){
	/// Cleanup Assets

}

void Scene0::Update(const float deltaTime){

	angle += 2;

	float rad = angle * (3.14159265359 / 180);

	//box->_transform->rotation.w = rad;

	bP->m_dynamicsWorld->stepSimulation(deltaTime);

	if (nM != nullptr)
		nM->Update();

	viewCamera->Update(deltaTime);

	for each(GameObject* object in GameObjects)
	{
		object->Update(deltaTime);
	}
}

void Scene0::Render() const{
	

	//First pass to write to the depth buffer
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	GLfloat near_plane = .1f, far_plane = 400.0f;
	lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	lightView = glm::lookAt(glm::vec3(dirLight->_transform->Position().x,
									  dirLight->_transform->Position().y,
									  dirLight->_transform->Position().z), glm::vec3(0.0f), glm::vec3(1.0));

	lightSpaceMatrix = lightProjection * lightView;


	dirLight->CreateDepthMap(GameObjects, lightSpaceMatrix);

	//Uniforms::SetLightSpaceMatrix(lightSpaceMatrix);
	//Bind the original frame buffer again
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/// Draw your scene here
	glViewport(0, 0, windowPtr->GetWidth(), windowPtr->GetHeight());
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Draw the skybox first
	glm::mat4 projectionMatrix = glm::perspective(45.0f, (float)windowPtr->GetWidth() / (float)windowPtr->GetHeight(), 0.1f, 100.f);
	glm::mat4 modelViewMatrix = glm::mat4(glm::mat3(viewCamera->GetViewMatrix()));

	skyboxShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "CameraViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	skybox->Render(skyboxShader);

	modelViewMatrix = viewCamera->GetViewMatrix();

	for (int i = 0; i <= ShaderManager::GetInstance()->GetNumShaders() - 1; i++)
	{
		Shader* shader = ShaderManager::GetInstance()->GetShader(i);
		if (shader){
			shader->Use();
			glUniformMatrix4fv(glGetUniformLocation(shader->Program, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader->Program, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader->Program, "CameraViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
			glUniform3f(glGetUniformLocation(shader->Program, "viewPos"), viewCamera->_transform->Position().x, viewCamera->_transform->Position().y, viewCamera->_transform->Position().z);

			glUniform1i(glGetUniformLocation(shader->Program, "shadowMap"), 5);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, dirLight->depthMap);

			glUniform1i(glGetUniformLocation(shader->Program, "skybox"), 6);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
		}
	}
	


	for each(GameObject* object in GameObjects)
	{
		object->Draw(object->GetShader());// , dirLight->depthMap, skybox->texture, object->reflective);
	}

	if (drawDebug){
		bP->m_dynamicsWorld->debugDrawWorld();
		((GLDebugDrawer*)bP->m_dynamicsWorld->getDebugDrawer())->DrawWorld(projectionMatrix, modelViewMatrix);
	}

	SDL_GL_SwapWindow(windowPtr->getSDLWindow());

}

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Scene0::HandleEvents(const SDL_Event& SDLEvent, float deltaTime){
	///std::cout << "event!!" << std::endl;

	

	switch (SDLEvent.type) {
		case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			if (SDLEvent.button.button == SDL_BUTTON_RIGHT){
				rightClickDown = true;
			}
			break;
		case SDL_EventType::SDL_MOUSEBUTTONUP:
			if (SDLEvent.button.button == SDL_BUTTON_RIGHT){
				rightClickDown = false;
				firstMouse = true; //Set this back to false so when you click again it doesn't make the camera jump.
			}
			break;
		case SDL_EventType::SDL_MOUSEMOTION:
			if (rightClickDown){
				if (firstMouse)
				{
					lastMouseX = SDLEvent.motion.x;
					lastMouseY = SDLEvent.motion.y;
					firstMouse = false;
				}

				viewCamera->ProcessMouseMovement(SDLEvent.motion.x - lastMouseX, lastMouseY - SDLEvent.motion.y);
				lastMouseX = SDLEvent.motion.x;
				lastMouseY = SDLEvent.motion.y;
			}
			break;
		case SDL_WINDOWEVENT:
			break;
		case SDL_KEYDOWN:
			if (!nM->isServer)
				nM->SendEvent(SDLEvent);
			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_W)
				viewCamera->moveForward = true;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_A)
				viewCamera->moveLeft = true;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_S)
				viewCamera->moveBackWard = true;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_D)
				viewCamera->moveRight = true;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_E)
				viewCamera->moveUp = true;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_Q)
				viewCamera->moveDown = true;

			break;
		case SDL_KEYUP:
			if (!nM->isServer)
				nM->SendEvent(SDLEvent);
			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_W)
				viewCamera->moveForward = false;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_A)
				viewCamera->moveLeft = false;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_S)
				viewCamera->moveBackWard = false;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_D)
				viewCamera->moveRight = false;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_E)
				viewCamera->moveUp = false;

			if (SDLEvent.key.keysym.scancode == SDL_SCANCODE_Q)
				viewCamera->moveDown = false;

			break;
		default:
			break;
	}
}

void Scene0::OnResize(int w_, int h_)
{
	windowPtr->SetWindowSize(w_, h_);
	glViewport(0, 0, windowPtr->GetWidth(), windowPtr->GetHeight());
}