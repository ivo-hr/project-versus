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
#include "../Utils/PlayerSelectRect.h"



class Entity;
class Character;
class Particle;

class HUDManager;

class MyListener;

class Stage;

class FightManager :public StateMachine
{

	std::vector<Entity*> entities;
	std::vector<Character*> characters;

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

	int cameraOffset = 75;

	void MoveCamera();

	vector<Texture*>winnersTextures;

	int winnerInput;
	vector<vector<int>>gameStats;

public:

	FightManager(SDLUtils* sdl, double screenAdjust);
	virtual ~FightManager();

	bool isExit() { return exit_; };
	bool isFinish() { return fightEnded; };
	void Update();

	void LoadStage(std::string file);
	int StartFight(std::vector<Character*> ent);
	int StartFight(std::vector<Character*> team1, std::vector<Character*> team2);
	int StartFight(std::vector<Character*> team1, std::vector<Character*> team2, std::vector<Character*> team3);
	void AddEntity(Entity* ent);
	bool RemoveEntity(Entity* ent);
	bool RemoveCharacter(Character* character);
	void MoveToFront(Entity* ent);
	void AddOponnent(Entity* ent, Entity* ignore = nullptr);

	void HitLag(int frames);

	void KillingBlow();

	void FighterLost(Character* loser);

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	SDL_Rect GetSDLCoors(float x, float y, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);
	int ToSDL(float x);

	int GetActualWidth();
	int GetActualHeight();

	double GetScreenRatio();
	double GetScreeAdjust();

	vector<Texture*>getWinnersTextures() { return winnersTextures; }

	std::vector<Entity*> GetOponents(Entity* current);
	SDL_Rect* GetDeathZone() ;
	b2World* GetWorld();
	SDLUtils* GetSDLU() { return sdl; };

	int getWinnerInput() { return winnerInput; }
	void addCharacterStats(Character* character);
	vector<vector<int>>getGameStats() { return gameStats; }

	void userExit() { exit_ = true; };

};
