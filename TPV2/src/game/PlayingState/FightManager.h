#pragma once

#include <string>

#include "../../../box2d/include/box2d/box2d.h"
#include <SDL.h>
#include "../../sdlutils/SDLUtils.h"
#include "../Character.h"
#include "../Entity.h"


class FightManager
{

	b2Body* stage;
	SDL_Rect* stageRect;

	std::vector<Entity*> entities;
	SDLUtils* sdl;
	b2World world;

	InputHandler* ih = InputHandler::instance();

	float b2ToSDL = 20.f;

public:
	FightManager();
	virtual ~FightManager();

	int StartFight(Character* p1, Character* p2);
	void AddEntity(Entity* ent);
	bool RemoveEntity(Entity* ent);
	void HitLag(int mSecs);

	SDL_Rect* GetSDLCoors(b2Body* body, b2Vec2* size);
};