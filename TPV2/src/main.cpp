// This file is part of the course Proyectos2@UCM - LeftOvers Productions �

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include "../CharInclude.h"
#include "sdlutils/InputHandler.h"
#include "sdlutils/macros.h"

#include "sdlutils/sdlutils_demo.h"
#include "sdlutils/SDLUtils.h"

#include "game/PlayingState/FightManager.h"

int main(int ac, char **av) {

	// Initialise the SDLGame singleton
	SDLUtils::init("Project Vs21", 512, 288,
		"resources/config/resources.json");

	auto& sdl = *SDLUtils::instance();

	//Obtenemos el tama�o de la pantalla
	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(0, &DM);

	/*
	float scaleX = (float)DM.w / sdl.width();
	float scaleY = (float)DM.h / sdl.height();

	SDL_RenderSetScale(sdl.renderer(), scaleX, scaleY);
	*/

	//Escalamos toda la ventana para que se ajuste al tama�o de la pantalla
	SDL_RenderSetLogicalSize(sdl.renderer(), DM.w, DM.h);

	//Cambiamos el tama�o de la ventana
	SDL_SetWindowSize(sdl.window(), DM.w, DM.h);

	//creamos el renderer
	SDL_Renderer* gRenderer = NULL;

	//Ponemos en pantalla completa
	//sdl.toggleFullScreen();

	//show the cursor
	sdl.showCursor();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto& ih = *InputHandler::instance();
	ih.initialiseJoysticks();
	ih.initialiseKeyboardState();
	//------------------------------------------------------------------------------------------

	FightManager* fghtmngr = new FightManager(&sdl);

	Character* character1 = new CharacterZero(fghtmngr,0);
	Character* boxingBag = new CharacterZero(fghtmngr,1);

	fghtmngr->StartFight(character1, boxingBag);
	

	return 0;
}
