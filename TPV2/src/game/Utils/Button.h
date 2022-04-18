#pragma once
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/InputHandler.h"

class Button
{
	InputHandler& ih = *InputHandler::instance();
	int x,y,w,h;
	Texture* tex;
	bool pressed;
public:

	Button():tex(nullptr),x(0),y(0),w(0),h(0),pressed(false) {};
	Button(Texture* t,int x,int y,int width,int height) :tex(t), x(x),y(y),w(width),h(height),pressed(false){};

	void setTexture(Texture* t) { tex = t; };
	void setX(int x0) { x = x0; };
	void setY(int y0) { y = y0; };
	void setW(int w0) { w = w0; };
	void setH(int h0) { h = h0; };
	void render();
	bool mouseClick();
	SDL_Rect getRect();
};

