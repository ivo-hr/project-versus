#pragma once
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include <iostream>
#include <string>

class Button
{
protected:
	SDLUtils* sdl = SDLUtils::instance();
	InputHandler& ih = *InputHandler::instance();
	int x,y,w,h;
	Texture* tex;
	Texture* destex = nullptr;
	Texture* presstex = nullptr;
	bool pressed;
	bool rendered = false;
	bool activated = false;
public:

	Button():tex(nullptr),x(0),y(0),w(0),h(0),pressed(false) {};
	Button(Texture* t,int x,int y,int width,int height) :tex(t), x(x),y(y),w(width),h(height),pressed(false){};
	Button(Texture* t,Texture*dt, int x, int y, int width, int height) :tex(t), x(x), y(y), w(width), h(height), pressed(false),destex(dt) {};
	
	void setPressTexture(Texture* t) { presstex = t; };
	void setTexture(Texture* t) { tex = t; };
	void setX(int x0) { x = x0; };
	void setY(int y0) { y = y0; };
	void setW(int w0) { w = w0; };
	void setH(int h0) { h = h0; };
	virtual void render();
	bool mouseClick();
	void setUnrendered() { rendered = false; };
	bool pointerClick(SDL_Rect rect);
	void active(bool b) { activated = b; };
	SDL_Rect getRect();
};

class PlayButton : public Button {
	std::vector<Texture*> txV;
	int cont = 0;
	int frame = 0;
public:
	PlayButton() :Button() {};
	PlayButton(Texture* t, int x, int y, int width, int height);
	void render() override;
};
