#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "SDL.h"
#include "Window.h"
#include "GameObject.h"
#include "Camera.h"
#include "Skybox.h"
#include "Light.h"
#include "NetworkManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "BulletPhysics.h"

namespace GAME {
	class Scene {
	public:
		explicit Scene(class Window& windowRef);
		virtual ~Scene();

		/*Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator = (const Scene&) = delete;
		Scene& operator = (Scene&&) = delete;*/
		
		

		/// Create the following four abstract functions and make them polymorphic
		/// The proper term for these are "pure viruals"
		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void Update(const float deltaTime) = 0;
		virtual void Render() const = 0;
		virtual void HandleEvents(const SDL_Event &SDLEvent, float deltaTime) = 0;
		virtual void OnResize(int w_, int h_) = 0;

		Shader* sceneShader;
		Shader* simpleDepthShader;
		Shader* skyboxShader;
		Shader* temp;

		Skybox* skybox;
		NetworkManager* nM;

		bool drawDebug;
		
		void CreateShadowBuffer() const;

	protected:
		Window* windowPtr;

		std::vector<GameObject*> GameObjects;
		std::vector<Light*> staticLights;
		
		Camera* viewCamera;
		

	};
}

#endif