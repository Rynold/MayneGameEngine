#ifndef Scene0_H
#define Scene0_H

#include "Scene.h"
#include "Model.h"
#include "Window.h"

namespace GAME {
	class Scene0 : public Scene  {
	protected:
		
	public:
		explicit Scene0(Window& windowRef);
		virtual ~Scene0();


		/// Delete these possible default constructors and operators  
		/*Scene0(const Scene0&) = delete;
		Scene0(Scene0 &&) = delete;
		Scene0& operator=(const Scene0 &) = delete;
		Scene0& operator=(Scene0 &&) = delete;*/

		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Update(const float deltaTime);
		virtual void Render() const;
		virtual void HandleEvents(const SDL_Event &SDLEvent, float deltaTime);
		/*virtual void KeyBoardEvent(const SDL_KeyboardEvent &keyEvent);
		virtual void MouseMotionEvent(const SDL_MouseMotionEvent &mouseEvent);*/
		virtual void OnResize(int w_, int h_);

		DirectionalLight* dirLight;

		GLuint depthMap;
		GLuint depthMapFBO;

		// = 1024;

		int lastMouseX, lastMouseY;
		bool firstMouse;// = true;
		bool rightClickDown;// = false;
		
		GameObject* floor_;
		GameObject* box;
		GameObject* reflectiveBox;

		std::shared_ptr<Shader> reflectiveShader;

		BulletPhysics* bP;
	};
}
#endif