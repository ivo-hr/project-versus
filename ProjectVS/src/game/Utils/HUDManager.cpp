#include "HUDManager.h"
#include "../Character.h"

HudManager::HudManager(FightManager* man): man(man) { Boot(man); }

void HudManager::Boot(FightManager* man)
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
		data[i].x = (int)(var->GetPNumber() * dist + offset);
		data[i].y = wHeight - (wHeight / 6);

		data[i].portraitRect = { data[i].x, data[i].y, wWidth / 14, wWidth / 14 };

		string player = "Player " + to_string((var->GetPNumber() + 1));
		SDL_Color c;
		if (var->GetPNumber() == 0) c = build_sdlcolor(0xFF000000);
		else if (var->GetPNumber() == 1) c = build_sdlcolor(0x002EFF00);
		else if (var->GetPNumber() == 2) c = build_sdlcolor(0x00FF6100);
		else if (var->GetPNumber() == 3) c = build_sdlcolor(0xFFF00000);
		else c = build_sdlcolor(0x00000000);
		data[i].pNumber = Texture(sdl->renderer(), player, *font, c);
		UpdateDmg(var, 0);
		UpdateLives(var, 3);
	}

}

void HudManager::UpdateLives(Character* var, const ushort& liv)
{
	string vidas = "Lives:" + to_string(liv);
	SDL_Color c;
	c = build_sdlcolor(0x00F7FF00);
	data[var->GetPNumber()].lives = Texture(sdl->renderer(), vidas, *font, c);
}

void HudManager::UpdateDmg(Character* a, const ushort& dmg)
{
	auto var = a->GetPNumber();
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

Texture& HudManager::PNumber(Character* a) { return data[a->GetPNumber()].pNumber; };
Texture& HudManager::Percent(Character* a) { return data[a->GetPNumber()].percent; };
Texture& HudManager::Lives(Character* a) { return data[a->GetPNumber()].lives; };
SDL_Rect& HudManager::PRect(Character* a) { return data[a->GetPNumber()].portraitRect; };
const int& HudManager::X(Character* a) { return data[a->GetPNumber()].x; };
const int& HudManager::Y(Character* a) { return data[a->GetPNumber()].y; };
