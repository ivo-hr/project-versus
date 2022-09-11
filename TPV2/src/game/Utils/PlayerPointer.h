#pragma once
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/SDLUtils.h"

class PlayerPointer {
	Texture* texture;
	int x, y, w, h , ww, wh;
	float vel;
	bool active = false;
	SDLUtils* sdl = SDLUtils::instance();
public:
	PlayerPointer() :texture(nullptr), x(0), y(0), w(0), h(0), wh(), ww(), vel((float)ww / 200.f) {};
	PlayerPointer(Texture* t, int x, int y, int ww, int wh) :texture(t), x(x), y(y), w(ww / 38), h(ww / 38), ww(ww), wh(wh), vel((float)ww / 200.f) {};
	virtual ~PlayerPointer() {};
	void render();
	void move(int dir);
	void setActive(bool b) { active = b; };
	bool getActive() { return active; };
	void setPosition(int newX, int newY);
	SDL_Rect getRect();
};