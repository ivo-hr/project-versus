#pragma once
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/SDLUtils.h"

class PlayerSelectRect
{
	Texture* back ,* front , *k1,*k2,*m;
	SDLUtils* sdl = SDLUtils::instance();
	bool gotInput;
	int cont=0;
	int tick = 0;
public:
	PlayerSelectRect(Texture* b);
	void setFront(Texture* f) { front = f; };
	void setgotInput(bool b) { gotInput = b; };
	void render(int x, int y, int w, int h);
	~PlayerSelectRect() {};
};

