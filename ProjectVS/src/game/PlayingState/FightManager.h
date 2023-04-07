#pragma once

#include <SDL.h>
//#include <iostream>
#include <box2d.h>
#include <vector>
#include <deque>
#include <queue>
#include <functional>

//#include "../../sdlutils/InputHandler.h"
//#include "../../sdlutils/macros.h"
//#include "../../sdlutils/SDLUtils.h"
#include "../State/StateMachine.h"
#include "../../utils/Vector2D.h"
//#include "../State/MenuState.h"
//#include "../State/PlayingState.h"
//#include "../State/GameOverState.h"
//#include "../Utils/PlayerSelectRect.h"

class Entity;
class Character;
class Particle;

class MyListener;

class Stage;

class HudManager;
class InputHandler;

enum class Tags
{
	CameraFollow,
	Hitable,
	IsProjectile,
	IsCharacter
};

class FightManager : public StateMachine
{
	std::deque<Entity*> entities = std::deque<Entity*>();
	std::deque<Character*> characters = std::deque<Character*>();

	std::deque<Entity*> camFollow = std::deque<Entity*>();

	std::queue<Entity*> toAdd = std::queue<Entity*>();

	std::vector<std::vector<Entity*>> entityMatrix;
	std::pair<ushort, short> ptrPlace = { 0, 0 };

	// Team mode
	std::deque<Character**> team1;
	std::deque<Character**> team2;
	std::vector<std::vector<ushort>>team1DeadStats;
	std::vector<std::vector<ushort>>team2DeadStats;
	std::vector<Texture*>team1DeadTextures;
	std::vector<Texture*>team2DeadTextures;

	SDLUtils* sdl;

	HudManager* hud = nullptr;

	InputHandler* ih;

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

	ushort cameraOffset = 70;
	Vector2D camShake;
	ushort shakeDuration;

	void MoveCamera();

	std::vector<Texture*> deadTextures; // Textures of dead characters for gameover stats

	short winnerInput;
	std::vector<std::vector<ushort>> gameStats;
	bool endGame = false;
	unsigned int endGameTimer = 0;
	bool teammode = false;

	void startCount();
	short scount = 4;
	unsigned int startticks = 0;

	std::deque<Entity*>::iterator RemoveEntity(std::deque<Entity*>::iterator ent);
	std::deque<Entity*>::iterator DeleteEntity(std::deque<Entity*>::iterator ent);
	bool DeleteCharacter(std::deque<Character*>::iterator character);
	void RemoveFromFollowCamera(Entity* ent);
	void AddToGame(Entity* ent);

public:

	FightManager();
	virtual ~FightManager();

	void InitMainLoop();

	bool isExit() { return exit_; };
	bool isFinish() { return fightEnded; };
	void Update();
	void DrawFight();

	void LoadStage(std::string file);
	void InitCamera();
	ushort StartFight(std::vector<Character*> ent);
	ushort StartFight(std::vector<Character*> team1, std::vector<Character*> team2);
	void InitMatrix();
	void RemoveEntityFromMatrix(Entity* ent);

	void AddEntity(Entity* ent);
	void AddEntity(Entity* ent, ushort layer);
	void FollowCamera(Entity* ent);
	bool RemoveEntity(Entity* ent);
	void MoveToFront(Entity* ent);

	void ChangeEntityLayer(Entity* ent, ushort newLayer);
	bool GetNextEntity(Entity*& ent, ushort layer);

	void KillingBlow();

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	SDL_Rect GetSDLCoors(float x, float y, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);

	int ToSDL(float x);
	float ToSDLf(float x);

	ushort GetActualWidth();
	ushort GetActualHeight();

	double GetScreenRatio();
	double GetScreeAdjust();

	std::vector<Texture*> getWinnersTextures() { return deadTextures; }

	void GetAllReferencesTo(Entity* toCheck, Entity*& entQue, Character*& chr, Entity*& cam, Entity*& mat);

	Entity*& GetEntityReferenceTo(Entity* toCheck);
	Character*& GetCharacterReferenceTo(Character* toCheck);
	Entity*& GetCameraReferenceTo(Entity* toCheck);
	Entity*& GetMatrixReferenceTo(Entity* toCheck);

	std::deque<Character*>& GetCharacters() { return characters; };

	ushort GetCharacterIndex(Character* toCheck) { for (auto i = 0u; i < characters.size(); i++) if (characters[i] == toCheck) return i; return USHRT_MAX; };
	Character* GetCharacter(ushort index) { return characters[index]; };

	SDL_Rect* GetDeathZone();
	b2Vec2* GetDeathZoneB2();
	b2World* GetWorld();
	MyListener* GetListener() { return listener; };
	SDLUtils* GetSDLU() { return sdl; };

	HudManager* GetHUD() { return hud; };

	short getWinnerInput() { return winnerInput; }
	void addCharacterStats(Character* character);
	std::vector<std::vector<ushort>>getGameStats() { return gameStats; }

	ushort GetNumOfPlayers() { return numPlayers; };

	void userExit() { exit_ = true; };

	void onNewGame();

	bool getTeammode() { return teammode; }

	void SetShake(const Vector2D& dir, ushort duration);

	void TakeScreenShot();

	void LogEverything();

};

