#pragma once

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include <vector>
#include <string>


#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/macros.h"
#include "../../sdlutils/SDLUtils.h"

class Entity;

class FightManager
{

	b2Body* stage;
	SDL_Rect stageRect;

	std::vector<Entity*> entities;
	SDLUtils* sdl;
	b2World world;

	InputHandler& ih = *InputHandler::instance();

	Entity* winner;
	bool fightEnded = false;

	float b2ToSDL = 20.f;

	int numPlayers = 2;

	vector<Texture*>winnersTextures;

public:
	FightManager(SDLUtils* sdl);
	virtual ~FightManager();

	int StartFight(Entity* p1, Entity* p2);
	void AddEntity(Entity* ent);
	bool RemoveEntity(Entity* ent);
	void HitLag(int mSecs);

	void FighterLost(Entity* loser);

	//vector<Character*>getWinners() { return characters; }
	vector<Texture*>getWinnersTextures() { return winnersTextures; }

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);


	std::vector<Entity*> GetOponents(Entity* current);
	b2World* GetWorld() { return &world; };
	SDLUtils* GetSDLU() { return sdl; };
};
