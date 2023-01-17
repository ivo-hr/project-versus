#pragma once
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Utils/PlayerConfigs.h"
#include "../../sdlutils/InputHandler.h"

class PlayerPointer
{
	SDLUtils* sdl = SDLUtils::instance();
	InputHandler& ih = *InputHandler::instance();
	PlayerConfigs& playerPrefs = *PlayerConfigs::instance();

	Texture* texture;
	int x, y, w, h , ww, wh;
	float vel;
	bool active = false;

	int input;

	bool clicked = false;
	bool clickedLastFrame = false;

public:
	PlayerPointer() :texture(nullptr), x(0), y(0), w(0), h(0), wh(), ww(), vel((float)ww / 200.f), input(-10) {};
	PlayerPointer(Texture* t, int x, int y, int ww, int wh, int input = -10) :texture(t), x(x), y(y), w(ww / 38), h(ww / 38), ww(ww), wh(wh), vel((float)ww / 200.f), input(input) {};
	virtual ~PlayerPointer() {};
	void render();
	void update();
	bool Click();
	void move(int dir);
	void setActive(bool b) { active = b; };
	bool getActive() { return active; };
	void setPosition(int newX, int newY);
	void setInput(short in) { input = in; };
	SDL_Rect getRect();
};