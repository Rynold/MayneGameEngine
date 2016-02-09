#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "SDL.h"
///
namespace GAME {

class Graphics {
public:

	/// C11 precautions delete these non-needed default constructors and operators
	Graphics(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	Graphics& operator = (Graphics&&) = delete;

	Graphics(SDL_Window const &window);
	~Graphics();

private:
	

	void getGLInfo();

};


}
#endif GRAPHICS_H
