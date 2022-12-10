#pragma once
#include <unordered_map>
#include "../PlayingState/FightManager.h"

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
	string fontstring;
	Font* font;
	string fontstringp;
	Font* fontp;

	FightManager* man;

	std::vector<HudData> data;

public:

	HudManager(FightManager* man): man(man) { Boot(man); }

	void Boot(FightManager* man)
	{
		wWidth = man->GetActualWidth();
		wHeight = man->GetActualHeight();

		dist = wWidth / man->GetNumOfPlayers();
		offset = (wWidth / 2) / man->GetNumOfPlayers() - wWidth / 30;

		sdl = man->GetSDLU();
		fontstring = "nes" + to_string(7 * wWidth / sdl->width());
		font = &sdl->fonts().at(fontstring);
		fontstringp = "nes" + to_string(10 * wWidth / sdl->width());
		fontp = &sdl->fonts().at(fontstringp);

		for (int i = 0; i < man->GetNumOfPlayers(); i++)
		{
			auto var = man->GetCharacter(i);
			data.push_back(HudData());
			data[i].x = (int)(man->GetCharacterIndex(var) * dist + offset);
			data[i].y = wHeight - (wHeight / 6);

			data[i].portraitRect = { data[i].x, data[i].y, wWidth / 14, wWidth / 14 };

			string player = "Player " + to_string((man->GetCharacterIndex(var) + 1));
			SDL_Color c;
			if (man->GetCharacterIndex(var) == 0) c = build_sdlcolor(0xFF000000);
			else if (man->GetCharacterIndex(var) == 1) c = build_sdlcolor(0x002EFF00);
			else if (man->GetCharacterIndex(var) == 2) c = build_sdlcolor(0x00FF6100);
			else if (man->GetCharacterIndex(var) == 3) c = build_sdlcolor(0xFFF00000);
			else c = build_sdlcolor(0x00000000);
			data[i].pNumber = Texture(sdl->renderer(), player, *font, c);
			UpdateDmg(var, 0);
			UpdateLives(var, 3);
		}

	}

	void UpdateLives(Character* var, const ushort& liv)
	{
		string vidas = "Lives:" + to_string(liv);
		SDL_Color c;
		c = build_sdlcolor(0x00F7FF00);
		data[man->GetCharacterIndex(var)].lives = Texture(sdl->renderer(), vidas, *font, c);
	}

	void UpdateDmg(Character* a, const ushort& dmg)
	{
		auto var = man->GetCharacterIndex(a);
		string damage = to_string(dmg) + "%";
		if (dmg == 0)
		{
			data[var].r = 0;
			data[var].g = 255;
		}
		else if (dmg < 255)
		{
			if (data[var].r <= 255) {
				data[var].r = dmg * 2;
				if (data[var].r > 255) data[var].r = 255;
			}
			else
			{
				data[var].g = 255 - dmg;
				if (data[var].g < 0) data[var].g = 0;
			}
		}
		else
		{
			Uint32 a = (Uint32)((((float)dmg - 255.f) / (float)SDL_MAX_SINT16) * 255 * 20);
			if (a < 255)
				data[var].r = 255 - a;
			else
				data[var].r = 0;
			data[var].g = 0;
		}
		Uint32 color = (Uint32)(data[var].r * pow(16, 6) + data[var].g * pow(16, 4));
		SDL_Color c = build_sdlcolor(color);
		data[var].percent = Texture(sdl->renderer(), damage, *fontp, c);
	}

	Texture& PNumber(Character* a) { return data[man->GetCharacterIndex(a)].pNumber; };
	Texture& Percent(Character* a) { return data[man->GetCharacterIndex(a)].percent; };
	Texture& Lives(Character* a) { return data[man->GetCharacterIndex(a)].lives; };
	SDL_Rect& PRect(Character* a) { return data[man->GetCharacterIndex(a)].portraitRect; };
	const int& X(Character* a) { return data[man->GetCharacterIndex(a)].x; };
	const int& Y(Character* a) { return data[man->GetCharacterIndex(a)].y; };
};
