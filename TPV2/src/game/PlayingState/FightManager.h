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

class Stage;

class FightManager :public StateMachine
{

	std::vector<Entity*> entities;
	std::vector<Entity*> characters;

	std::vector<Particle*> particulas;

	SDLUtils* sdl;

	InputHandler& ih = *InputHandler::instance();
	HUDManager* hud;

	Entity* winner;

	double screenAdjust;

	bool fightEnded = false;

	int numPlayers = 2;
	
	int addedDelay;

	bool exit_ = false;

	MyListener* listener;
	
	float step = 1.f / 60.f;

	Stage* stage;

	SDL_Rect camera; 
	SDL_Rect hitLagCam;

	int cameraOffset = 300;

	void MoveCamera();

public:

	FightManager(SDLUtils* sdl, double screenAdjust);
	virtual ~FightManager();

	bool isExit() { return exit_; };
	bool isFinish() { return fightEnded; };
	void Update();
	int StartFight(std::vector<Entity*> ent);
	void AddEntity(Entity* ent);
	bool RemoveEntity(Entity* ent);
	bool RemoveCharacter(Entity* character);
	void MoveToFront(Entity* ent);
	void AddOponnent(Entity* ent, Entity* ignore = nullptr);

	void AddParticle(Particle* par);
	bool RemoveParticle(Particle* par);
	void HitLag(int frames);

	void KillingBlow(Vector2D dead);

	void FighterLost(Entity* loser);

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	SDL_Rect GetSDLCoors(float x, float y, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);
	int ToSDL(float x);

	int GetActualWidth();
	int GetActualHeight();

	double GetScreenRatio();
	double GetScreeAdjust();

	std::vector<Entity*> GetOponents(Entity* current);
	SDL_Rect* GetDeathZone() ;
	b2World* GetWorld();
	SDLUtils* GetSDLU() { return sdl; };
};
