// This file is part of the course Proyectos2@UCM - LeftOvers Productions �

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include <cstdlib>
#include "sdlutils/InputHandler.h"
#include "sdlutils/macros.h"

#include "utils/CheckML.h"

#include "sdlutils/sdlutils_demo.h"
#include "sdlutils/SDLUtils.h"

#include "game/PlayingState/FightManager.h"

int main(int ac, char **av) {

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
#endif // _DEBUG

	// Initialise the SDLGame singleton
	SDLUtils::init("Project Vs21", 512, 288,
		"resources/config/resources.json");

	auto& sdl = *SDLUtils::instance();

	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(0, &DM);

	//Obtenemos el tama�o de la pantalla

	/*
	float scaleX = (float)DM.w / sdl.width();
	float scaleY = (float)DM.h / sdl.height();

	SDL_RenderSetScale(sdl.renderer(), scaleX, scaleY);
	*/

	//Escalamos toda la ventana para que se ajuste al tama�o de la pantalla

	//Cambiamos el tama�o de la ventana

	//SDL_SetWindowPosition(sdl.window(), 0, 5);
	//Ponemos en pantalla completa

	SDL_MaximizeWindow(sdl.window());
	SDL_SetWindowSize(sdl.window(), DM.w, DM.h);
	SDL_RenderSetLogicalSize(sdl.renderer(), DM.w, DM.h);
	sdl.toggleFullScreen();

#ifdef _DEBUG

	sdl.toggleFullScreen();
	//SDL_MaximizeWindow(sdl.window());
	int w, h;
	SDL_GetWindowSize(sdl.window(), &w, &h);
	SDL_RenderSetLogicalSize(sdl.renderer(), w, h);

	Music::setMusicVolume((int)(1));
	SoundEffect::setChannelVolume((int)(1));

#endif // _DEBUG

	SDL_SetWindowResizable(sdl.window(), SDL_TRUE);

	//show the cursor
	sdl.showCursor();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto& ih = *InputHandler::instance();
	ih.initialiseJoysticks();
	//------------------------------------------------------------------------------------------

	FightManager* fghtmngr = nullptr;

	try
	{
		fghtmngr = new FightManager(&sdl);
#ifdef _DEBUG
		fghtmngr->LogEverything();
#endif // !_DEBUG
	}
	catch (const char* s)
	{
		const char* title = "The game crashed :(     ";

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, s, nullptr);
	}
	delete fghtmngr;

	SDL_Quit();

	return 0;
}
