#include "GL/glew.h"
#include <SDL.h>
#include "Scene0.h"
#include "gtc\type_ptr.hpp"
#include <iostream>
using namespace GAME;

void RenderQuad();

Scene0::Scene0(class Window& windowRef):  Scene(windowRef) {
	viewCamera = new Camera(glm::vec3(0.0, 0.0, 4.0), glm::vec3(0.0, 1.0, 0.0), -90, 0);
	//sceneShader = new Shader("Shaders/TBNShader.vert", "Shaders/TBNShader.frag");
	//sceneShader = new Shader("texture.vert", "texture.frag");
	sceneShader = new Shader("Shaders/ParallaxShader.vert", "Shaders/ParallaxShader.frag");
	simpleDepthShader = new Shader("triangles.vert", "triangles.frag");
	skyboxShader = new Shader("Shaders/skybox.vert", "Shaders/skybox.frag");
	sceneShader->Use();
	pointLight = new PointLight();
	dirLight = new DirectionalLight();
	skybox = new Skybox();

	char playerChoice[10];
	bool properChoice = false;

	while (!properChoice){
		

		std::cout << "Start as Server? (Y/N):" << std::endl;
		std::cin >> playerChoice;

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

}

Scene0::~Scene0(){ 
	delete pointLight;
	pointLight = nullptr;

	delete dirLight;
	dirLight = nullptr;


	glDeleteFramebuffers(1, &this->depthMapFBO);
	// Skybox memory is freed from parent class
}

bool Scene0::OnCreate() {
	OnResize(windowPtr->GetWidth(), windowPtr->GetHeight());
	/// Load Assets: as needed 
	sceneShader->Use();
	
	
	floor_ = new GameObject();
	floor_->_transform->position = glm::vec3(0.0, -2.0, 0.0);
	floor_->_transform->rotation = glm::vec4(0, 0, 1, 0);
	floor_->AddComponent(new Mesh(EMeshType::PLANE));
	GameObjects.push_back(floor_);

	box = new GameObject();
	box->AddComponent(new Mesh(EMeshType::CUBE));
	box->_transform->position = glm::vec3(0,0,0);
	box->_transform->rotation = glm::vec4(1,0,1,0);
	GameObjects.push_back(box);

	nM->AttachListener(viewCamera);

	pointLight->_transform->position.x = 0.0f;// = new Vec3(-1.0, 3.0, 5.0);
	pointLight->_transform->position.y = 0.0f;
	pointLight->_transform->position.z = 2.0f;

	dirLight->_transform->position.x = -4.0f;
	dirLight->_transform->position.y = 8.0f;
	dirLight->_transform->position.z = -2.0f;

	sceneShader->Use();

	// Directional light
	glUniform3f(glGetUniformLocation(sceneShader->Program, "dirLight.position"), dirLight->_transform->position.x, dirLight->_transform->position.y, dirLight->_transform->position.z);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "dirLight.direction"), 0.0f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "dirLight.specular"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "dirLight.color"), 0.8f, .8f, .8f);


	glUniform3f(glGetUniformLocation(sceneShader->Program, "pointLights[0].position"), pointLight->_transform->position.x, pointLight->_transform->position.y, pointLight->_transform->position.z);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "pointLights[0].color"), 0.0, .8f, .8f);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "pointLights[0].ambient"), 0.0f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "pointLights[0].diffuse"), 0.0f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "pointLights[0].specular"), 0.0f, 0.8f, 0.8f);
	glUniform1f(glGetUniformLocation(sceneShader->Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(sceneShader->Program, "pointLights[0].linear"), 0.09);
	glUniform1f(glGetUniformLocation(sceneShader->Program, "pointLights[0].quadratic"), 0.032);

	//create our own frame buffer to use for shadows
	glGenFramebuffers(1, &depthMapFBO);

	//Generate the texture used for shadows
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	return true;
}

void Scene0::OnDestroy(){
	/// Cleanup Assets

}

void Scene0::Update(const float deltaTime){

	//pointLight->_transform->position.x -= 0.001;

	if (nM != nullptr)
		nM->Update();

	viewCamera->Update(deltaTime);

	for each(GameObject* object in GameObjects)
	{
		object->Update(deltaTime);
	}

	box->_transform->rotation.w += .01;
}

void Scene0::Render() const{

	//First pass to write to the depth buffer
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	GLfloat near_plane = .1f, far_plane = 400.0f;
	lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	lightView = glm::lookAt(glm::vec3(dirLight->_transform->position.x,
									  dirLight->_transform->position.y, 
									  dirLight->_transform->position.z), glm::vec3(0.0f), glm::vec3(1.0));

	lightSpaceMatrix = lightProjection * lightView;

	simpleDepthShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader->Program, "lightSpaceMatrix"),1,GL_FALSE,glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	//Render each object with the shadow shader for the depth
	for each(GameObject* object in GameObjects)
	{
		object->Draw(simpleDepthShader, depthMap);
	}

	//Bind the original frame buffer again
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/// Draw your scene here
	glViewport(0, 0, windowPtr->GetWidth(), windowPtr->GetHeight());
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Draw the skybox first
	glm::mat4 projectionMatrix =glm::perspective(45.0f, (float)windowPtr->GetWidth() / (float)windowPtr->GetHeight(), 0.1f, 100.f);
	glm::mat4 modelViewMatrix = glm::mat4(glm::mat3(viewCamera->GetViewMatrix()));

	skyboxShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "projection"), 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "view"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	skybox->Render(skyboxShader);

	sceneShader->Use();
	modelViewMatrix = viewCamera->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(sceneShader->Program, "CameraViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	glUniform3f(glGetUniformLocation(sceneShader->Program, "viewPos"), viewCamera->_transform->position.x, viewCamera->_transform->position.y, viewCamera->_transform->position.z);

	for each(GameObject* object in GameObjects)
	{
		object->Draw(sceneShader, depthMap);
	}

	//glFlush();

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
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 10.0, 0.0, 10.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
	//glTranslatef(5.0, 5.0, 0.0);
}