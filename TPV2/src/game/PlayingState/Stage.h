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
	b2Body* stage;
	SDL_Rect stageRect;
	b2Body* platform;
	SDL_Rect platformRect;

	SDLUtils* sdl;
	b2World world;

	SDL_Rect deathZone;
	float b2ToSDL;

	MyListener* listener;
	Texture* background;
	Texture* platformTexture;

	float deathzoneSize = 75.f;
	float step;

public:
	Stage(SDLUtils* sdl, MyListener* _listener, double screenAdjust, float step, std::string filename);
	~Stage();

	void Update();

	double GetScreenRatio() { return (b2ToSDL * 50) / sdl->width(); }

	SDL_Rect* GetDeathZone() { return &deathZone; };
	b2World* GetWorld() { return &world; };
	SDLUtils* GetSDLU() { return sdl; }

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	SDL_Rect GetSDLCoors(float x, float y, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);
	float getb2ToSDL() { return b2ToSDL; };
};

