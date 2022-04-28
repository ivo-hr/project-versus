#pragma once
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/SDLUtils.h"

class PlayerPointer {
	Texture* texture;
	int x, y, w, h ,vel ,ww,wh;
	bool active = false;
	SDLUtils* sdl = SDLUtils::instance();
public:
	PlayerPointer() :texture(nullptr), x(0), y(0), w(0), h(0),vel(1) {};
	PlayerPointer(Texture* t, int x, int y, int width, int height ,int ww,int wh) :texture(t), x(x), y(y), w(width), h(height) ,vel(8) ,ww(ww),wh(wh){};
	~PlayerPointer() {};
	void render();
	void move(int dir);
	void setActive(bool b) { active = b; };
	bool getActive() { return active; };
	void setPosition(int newX, int newY);
	SDL_Rect getRect();
};