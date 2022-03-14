#pragma once

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include <string>
#include <vector>

#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/macros.h"
#include "../../sdlutils/SDLUtils.h"


class Entity;

class HUDManager;

class MyListener;

class FightManager
{

	b2Body* stage;
	SDL_Rect stageRect;

	std::vector<Entity*> entities;
	SDLUtils* sdl;
	b2World world;

	InputHandler& ih = *InputHandler::instance();
	HUDManager* hud;

	Entity* winner;

	SDL_Rect deathZone;

	bool fightEnded = false;

	float b2ToSDL;

	int numPlayers = 2;
	
	int addedDelay;

	MyListener* listener;
	

public:
	FightManager(SDLUtils* sdl, double screenAdjust);
	virtual ~FightManager();

	int StartFight(Entity* p1, Entity* p2);
	void AddEntity(Entity* ent);
	bool RemoveEntity(Entity* ent);
	void HitLag(int frames);

	void FighterLost(Entity* loser);

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);


	std::vector<Entity*> GetOponents(Entity* current);
	b2World* GetWorld() { return &world; };
	SDLUtils* GetSDLU() { return sdl; };
};
