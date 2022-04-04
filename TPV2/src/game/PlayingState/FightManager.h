#pragma once

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include <string>
#include <vector>

#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/macros.h"
#include "../../sdlutils/SDLUtils.h"
#include "../State/StateMachine.h"
#include "../State/MenuState.h"
#include "../State/PlayingState.h"
#include "../State/GameOverState.h"
#include "../State/PauseState.h"


class Entity;
class Particle;

class HUDManager;

class MyListener;

class FightManager:public StateMachine
{

	b2Body* stage;
	SDL_Rect stageRect;
	b2Body* platform;
	SDL_Rect platformRect;

	std::vector<Entity*> entities;
	std::vector<Particle*> particulas;

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

	bool exit_ = false;

	MyListener* listener;
	
	Texture* background;
	Texture* testura;

public:
	FightManager(SDLUtils* sdl, double screenAdjust);
	virtual ~FightManager();

	bool isExit() { return exit_; };
	bool isFinish() { return fightEnded; };
	void Update();
	int StartFight(Entity* p1, Entity* p2);
	void AddEntity(Entity* ent);
	bool RemoveEntity(Entity* ent);
	void MoveToFront(Entity* ent);

	void AddParticle(Particle* par);
	bool RemoveParticle(Particle* par);
	void HitLag(int frames);

	void KillingBlow(Vector2D dead);

	void FighterLost(Entity* loser);

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	SDL_Rect GetSDLCoors(float x, float y, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);
	int ToSDL(float x) {
		return x * b2ToSDL;
	};

	double GetScreenRatio() { return (b2ToSDL * 50) / sdl->width(); };

	std::vector<Entity*> GetOponents(Entity* current);
	SDL_Rect* GetDeathZone() { return &deathZone; };
	b2World* GetWorld() { return &world; };
	SDLUtils* GetSDLU() { return sdl; };
};
