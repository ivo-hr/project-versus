// This file is part of the course Proyectos2@UCM - LeftOvers Productions �

//#include <SDL.h>
//#include <iostream>
//#include <box2d.h>
//#include <cstdlib>
//
//#include "sdlutils/InputHandler.h"
//#include "sdlutils/macros.h"
//#include "sdlutils/sdlutils_demo.h"
//#include "sdlutils/SDLUtils.h"

#include "game/PlayingState/FightManager.h"
#include "utils/CheckML.h"

int main(int ac, char **av) {

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
#endif // _DEBUG

	int maxRenderWidth = 1680;

	// Initialise the SDLGame singleton
	SDLUtils::init("Project Vs21", 512, 288,
		"resources/config/resources.json");

	auto& sdl = *SDLUtils::instance();

	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(0, &DM);

	Vector2D size;

	if (maxRenderWidth < DM.w)
	{
		double ratio = (double)DM.h / (double)DM.w;
		size = Vector2D(maxRenderWidth, (int)(maxRenderWidth * ratio));

		SDL_SetWindowSize(sdl.window(), size.getX(), size.getY());
	}
	else
	{
		size = Vector2D(DM.w, DM.h);
		SDL_MaximizeWindow(sdl.window());
	}

	// SDL_SetWindowPosition(sdl.window(), 0, 40);

	// SDL_MaximizeWindow(sdl.window());

	SDL_SetWindowResizable(sdl.window(), SDL_TRUE);

	// SDL_MaximizeWindow(sdl->window());
	// sdl->toggleFullScreen();
#ifndef _DEBUG
	// SDL_MaximizeWindow(sdl->window());
	sdl.toggleFullScreen();
#endif

	sdl.showCursor();

	Music::setMusicVolume((int)(1));
	SoundEffect::setChannelVolume((int)(1));

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto& ih = *InputHandler::instance();
	ih.initialiseJoysticks();
	//------------------------------------------------------------------------------------------

	FightManager* fghtmngr = nullptr;
	fghtmngr = new FightManager(&sdl);

	try
	{
		fghtmngr->InitMainLoop();
	}
	catch (const char* s)
	{
		const char* title = "The game crashed :(";

		if (s == nullptr)
			s = "And we don't know why";

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title, s, sdl.window());

		return -1;
	}
	delete fghtmngr;
	SDL_Quit();

	return 0;
}
