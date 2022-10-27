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
#include "../Utils/PlayerSelectRect.h"
#include "../State/ConfigurationState.h"

class Entity;
class Character;
class Particle;

class MyListener;

class Stage;

enum class Tags
{
	CameraFollow,
	Hitable,
	IsProjectile,
	IsCharacter
};

class FightManager : public StateMachine
{

	std::vector<Entity*> entities = vector<Entity*>(0);
	std::vector<Character*> characters = vector<Character*>(0);

	std::vector<Entity*> camFollow = vector<Entity*>(0);

	std::vector<vector<Entity*>> entityMatrix;
	std::pair<ushort, short> ptrPlace = { 0, 0 };

	// Team mode
	std::vector<Character*> team1;
	std::vector<Character*> team2;
	vector<vector<ushort>>team1DeadStats;
	vector<vector<ushort>>team2DeadStats;
	vector<Texture*>team1DeadTextures;
	vector<Texture*>team2DeadTextures;

	SDLUtils* sdl;

	InputHandler& ih = *InputHandler::instance();

	Entity* winner;

	ushort width, height;
	float sizeDiff;

	bool fightEnded = false;

	short numPlayers = 0;
	
	ushort addedDelay;

	bool exit_ = false;

	MyListener* listener;
	
	float step = 1.f / 60.f;

	Stage* stage;

	SDL_Rect camera; 
	SDL_Rect auxCam;

	ushort cameraOffset = 75;
	Vector2D camShake;
	uint16 shakeDuration;

	void MoveCamera();

	vector<Texture*> deadTextures; // Textures of dead characters for gameover stats

	short winnerInput;
	vector<vector<ushort>> gameStats;
	bool endGame = false;
	unsigned int endGameTimer = 0;
	bool teammode = false;

	void startCount();
	short scount = 4;
	unsigned int startticks = 0;
public:

	FightManager(SDLUtils* sdl);
	virtual ~FightManager();

	bool isExit() { return exit_; };
	bool isFinish() { return fightEnded; };
	void Update();

	void LoadStage(std::string file);
	void InitCamera();
	ushort StartFight(std::vector<Character*> ent);
	ushort StartFight(std::vector<Character*> team1, std::vector<Character*> team2);
	void InitMatrix();
	void RemoveEntityFromMatrix(Entity* ent);

	void AddEntity(Entity* ent);
	void AddEntity(Entity* ent, ushort layer);
	void FollowCamera(Entity* ent);
	void RemoveFromFollowCamera(Entity* ent);
	bool RemoveEntity(Entity* ent, bool shouldDelete = true);
	bool RemoveCharacter(Character* character);
	void MoveToFront(Entity* ent);

	void ChangeEntityLayer(Entity* ent, ushort newLayer);
	bool GetNextEntity(Entity*& ent, ushort layer);

	void KillingBlow();

	void FighterLost(Character* loser);

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	SDL_Rect GetSDLCoors(float x, float y, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);
	int ToSDL(float x);

	ushort GetActualWidth();
	ushort GetActualHeight();

	double GetScreenRatio();
	double GetScreeAdjust();

	vector<Texture*>getWinnersTextures() { return deadTextures; }

	std::vector<Entity*>* GetEntities();
	SDL_Rect* GetDeathZone();
	b2Vec2* GetDeathZoneB2();
	SDL_Rect* GetBubbleDeathZone();
	b2Vec2* GetBubbleDeathZoneB2();
	b2World* GetWorld();
	SDLUtils* GetSDLU() { return sdl; };

	short getWinnerInput() { return winnerInput; }
	void addCharacterStats(Character* character);
	vector<vector<ushort>>getGameStats() { return gameStats; }

	void userExit() { exit_ = true; };

	void onNewGame();

	bool getTeammode() { return teammode; }

	void SetShake(const Vector2D& dir, uint16 duration);

};
