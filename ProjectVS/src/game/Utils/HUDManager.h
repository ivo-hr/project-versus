#pragma once
#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/Texture.h"
#include <vector>

class FightManager;
class Character;

struct HudData
{
	short r = 0;
	short g = 255;

	bool init = false;

	int x = 0;
	int y = 0;

	Texture percent;
	Texture pNumber;
	Texture lives;
	SDL_Rect portraitRect = SDL_Rect();
};

class HudManager
{
private:
	int wWidth;
	int wHeight;
	int dist;
	int offset;

	SDLUtils* sdl;
	std::string fontstring;
	Font* font;
	std::string fontstringp;
	Font* fontp;

	FightManager* man;

	std::vector<HudData> data;

public:

	HudManager(FightManager* man);

	void Boot(FightManager* man);

	void UpdateLives(Character* var, const unsigned short& liv);

	void UpdateDmg(Character* a, const unsigned short& dmg);

	Texture& PNumber(Character* a);
	Texture& Percent(Character* a);
	Texture& Lives(Character* a);
	SDL_Rect& PRect(Character* a);
	const int& X(Character* a);
	const int& Y(Character* a);
};
