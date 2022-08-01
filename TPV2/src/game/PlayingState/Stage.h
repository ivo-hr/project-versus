#pragma once

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include <string>
#include <vector>
#include "../../sdlutils/SDLUtils.h"

#include "FightManager.h"

class MyListener;

class Stage
{
	FightManager* mngr;
	b2Body* stage;
	SDL_Rect stageRect;

	std::vector<b2Body*> platforms;
	std::vector<SDL_Rect> platformRects;

	std::vector<b2Vec2> playerSpawns;

	SDLUtils* sdl;
	b2World* world;

	SDL_Rect deathZone;
	float b2ToSDL;

	MyListener* listener;
	Texture* background;
	Texture* platformTexture;

	b2Vec2 deathzoneSize;
	float step;
	void reset();

public:
	Stage(FightManager* mngr, SDLUtils* sdl, MyListener* _listener, float step);
	virtual ~Stage();

	void UnLoadStage();

	void LoadJsonStage(std::string file, int width, int height);

	b2Vec2 GetPlayerSpawns(int index) { return playerSpawns[index]; };
	int GetPlayerDir(int index);

	void Update();
	void Update(SDL_Rect* camera);

	double GetScreenRatio() { return (b2ToSDL * 50) / sdl->width(); }

	SDL_Rect* GetDeathZone() { return &deathZone; };
	b2World* GetWorld() { return world; };
	SDLUtils* GetSDLU() { return sdl; }

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	SDL_Rect GetSDLCoors(float x, float y, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);
	float getb2ToSDL() { return b2ToSDL; };
};

