#include <iostream>
#include "GameSceneManager.h"
#include "Debug.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace GAME;

int main(int argc, char* argcs[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/// c11: This is evaluated at compile time, if void* != 4 then
	/// this must be a 64-bit build - just a quick test
	/// All my libraries are 32bit
	static_assert(sizeof(void*) == 4, "This program is not ready for 64-bit build");


	Debug::Init();
	GameSceneManager::getInstance()->Run();

	_CrtDumpMemoryLeaks();

	exit(0);
}