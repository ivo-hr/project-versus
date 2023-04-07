// This file is part of the course Proyectos2@UCM - LeftOvers Productions �

//#include <SDL.h>
//#include <iostream>
//#include <box2d.h>
//#include <cstdlib>
//
//#include "sdlutils/InputHandler.h"
//#include "sdlutils/macros.h"
//#include "sdlutils/sdlutils_demo.h"
#include "sdlutils/SDLUtils.h"
#include "game/PlayingState/FightManager.h"
#include "utils/CheckML.h"

int main(int ac, char **av) {

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
#endif // _DEBUG

	FightManager* fghtmngr = nullptr;
	fghtmngr = new FightManager();

	try
	{
		fghtmngr->InitMainLoop();
	}
	catch (const char* s)
	{
		const char* title = "The game crashed :(";

		if (s == nullptr)
			s = "And we don't know why";

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title, s, SDLUtils::instance()->window());

		return -1;
	}
	delete fghtmngr;
	SDL_Quit();

	return 0;
}
