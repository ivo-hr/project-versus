#pragma once
#include "../../sdlutils/SDLUtils.h"
#include "PlayerPointer.h"
#include <functional>
#include <vector>

class Button
{
protected:
	SDLUtils* sdl = SDLUtils::instance();
	InputHandler& ih = *InputHandler::instance();

	std::vector<PlayerPointer*>& pointers;

	int x,y,w,h;

	Texture* tex = nullptr;
	bool hasHoverSprite = false;
	bool hasPressedSprite = false;

	SDL_Rect defTex = { 0,0,0,0 };
	SDL_Rect hoverTex = { 0,0,0,0 };
	SDL_Rect pressTex = { 0,0,0,0 };

	SDL_Rect currentSprite = { 0,0,0,0 };

	bool pressed = false;
	bool rendered = false;

	bool validPointers = true;

	bool MouseOver();
	bool MouseEnter();
	bool MouseExit();

	bool mouseOver = false;
	bool mouseOverRemember = false;

	bool pressedLeft = false;
	bool isClicking = false;

	static const Uint8 defaultTint = 255;
	static const Uint8 hoverOverTint = 210;
	static const Uint8 pressedTint = 180;

	std::function<void()> onMouseClick = nullptr;
	std::function<void(int input)> onPointerClick = nullptr;

	bool hasBeenDeleted = false;

	bool active = true;

public:
	Button(Texture* t, int x, int y, int size, std::vector<PlayerPointer*>& p, bool hasHoverOver = false, bool hasPressed = false);
	Button(Texture* t, int x, int y, int w, int h, std::vector<PlayerPointer*>& p, bool hasHoverOver = false, bool hasPressed = false);

	virtual ~Button()
	{
		tex->SetTexMod(defaultTint);
		SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
		hasBeenDeleted = true;
	};

	void SetNoPointers() { validPointers = false; };

	void SetOnClick(std::function<void()> a) { onMouseClick = a; };
	void SetOnPointerClick(std::function<void(int)> a) { onPointerClick = a; };

	void SetActive(bool f);

	void setTexture(Texture* t) { tex = t; };
	void setX(int x0) { x = x0; };
	void setY(int y0) { y = y0; };
	void setW(int w0) { w = w0; };
	void setH(int h0) { h = h0; };
	void update();
	virtual void render();
	void setUnrendered() { rendered = false; };
	SDL_Rect getRect();
	virtual void reset() {};
};


class ToggleButton : public Button
{
	bool enabled = true;
public:
	ToggleButton(Texture* t, int x, int y, float size, std::vector<PlayerPointer*>& p, bool hasHoverOver = false, bool hasPressed = false);
	ToggleButton(Texture* t, int x, int y, int w, int h, std::vector<PlayerPointer*>& p, bool hasHoverOver = false, bool hasPressed = false);
	virtual void render() override;
	void SetEnabled(bool a);
};


class PlayButton : public Button {
	std::vector<Texture*> txV;
	unsigned short cont = 0;
	int frame = 0;
public:
	PlayButton(Texture* t, int x, int y, float size, std::vector<PlayerPointer*>& p, bool hasHoverOver = false, bool hasPressed = false);
	PlayButton(Texture* t, int x, int y, int w, int h, std::vector<PlayerPointer*>& p, bool hasHoverOver = false, bool hasPressed = false);
	virtual ~PlayButton() {};
	void render() override;
	void reset() override { frame = 0; };
};
