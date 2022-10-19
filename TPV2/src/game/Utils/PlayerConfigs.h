#pragma once
#include "../../sdlutils/InputHandler.h"

struct Buttons
{
public:
	SDL_GameControllerButton basic;
	SDL_GameControllerButton special;
	SDL_GameControllerButton jump;
	SDL_GameControllerButton taunt;

	Buttons() {
		basic = SDL_CONTROLLER_BUTTON_B; 
		special = SDL_CONTROLLER_BUTTON_A; 
		jump = SDL_CONTROLLER_BUTTON_X; 
		taunt = SDL_CONTROLLER_BUTTON_Y;
	}
};

struct KeyBoardKeys
{
public:
	unsigned int up;
	unsigned int down;
	unsigned int left;
	unsigned int right;
	unsigned int basic;
	unsigned int special;
	unsigned int jump;
	unsigned int taunt;

	KeyBoardKeys(bool leftKeys = true) {
		if (leftKeys)
		{
			up = SDLK_w;
			down = SDLK_s;
			left = SDLK_a;
			right = SDLK_d;
			basic = SDLK_LCTRL;
			special = SDLK_LSHIFT;
			jump = SDLK_w;
			taunt = SDLK_t;
		}
		else
		{
			up = SDLK_UP;
			down = SDLK_DOWN;
			left = SDLK_LEFT;
			right = SDLK_RIGHT;
			basic = SDLK_RCTRL;
			special = SDLK_RSHIFT;
			jump = SDLK_UP;
			taunt = SDLK_m;
		}
	}
};

class PlayerConfigs : public Singleton<PlayerConfigs>
{
private:

	KeyBoardKeys keyBoard1;
	KeyBoardKeys keyBoard2;
	Buttons controllerBut;

public:
	PlayerConfigs() { keyBoard1 = KeyBoardKeys(); keyBoard2 = KeyBoardKeys(false); controllerBut = Buttons(); };
	~PlayerConfigs() {};

	unsigned int Keyboard1Up() { return keyBoard1.up; }
	unsigned int Keyboard1Down() { return keyBoard1.down; }
	unsigned int Keyboard1Left() { return keyBoard1.left; }
	unsigned int Keyboard1Right() { return keyBoard1.right; }
	unsigned int Keyboard1Basic() { return keyBoard1.basic; }
	unsigned int Keyboard1Special() { return keyBoard1.special; }
	unsigned int Keyboard1Jump() { return keyBoard1.jump; }
	unsigned int Keyboard1Taunt() { return keyBoard1.taunt; }

	unsigned int Keyboard2Up() { return keyBoard2.up; }
	unsigned int Keyboard2Down() { return keyBoard2.down; }
	unsigned int Keyboard2Left() { return keyBoard2.left; }
	unsigned int Keyboard2Right() { return keyBoard2.right; }
	unsigned int Keyboard2Basic() { return keyBoard2.basic; }
	unsigned int Keyboard2Special() { return keyBoard2.special; }
	unsigned int Keyboard2Jump() { return keyBoard2.jump; }
	unsigned int Keyboard2Taunt() { return keyBoard2.taunt; }

	unsigned int KeyboardUp(bool leftKeys = true) { return leftKeys ? keyBoard1.up : keyBoard2.up; }
	unsigned int KeyboardDown(bool leftKeys = true) { return leftKeys ? keyBoard1.down : keyBoard2.down; }
	unsigned int KeyboardLeft(bool leftKeys = true) { return leftKeys ? keyBoard1.left : keyBoard2.left; }
	unsigned int KeyboardRight(bool leftKeys = true) { return leftKeys ? keyBoard1.right : keyBoard2.right; }
	unsigned int KeyboardBasic(bool leftKeys = true) { return leftKeys ? keyBoard1.basic : keyBoard2.basic; }
	unsigned int KeyboardSpecial(bool leftKeys = true) { return leftKeys ? keyBoard1.special : keyBoard2.special; }
	unsigned int KeyboardJump(bool leftKeys = true) { return leftKeys ? keyBoard1.jump : keyBoard2.jump; }
	unsigned int KeyboardTaunt(bool leftKeys = true) { return leftKeys ? keyBoard1.taunt : keyBoard2.taunt; }

	SDL_GameControllerButton ControllerBasic() { return controllerBut.basic; }
	SDL_GameControllerButton ControllerSpecial() { return controllerBut.special; }
	SDL_GameControllerButton ControllerJump() { return controllerBut.jump; }
	SDL_GameControllerButton ControllerTaunt() { return controllerBut.taunt; }
};