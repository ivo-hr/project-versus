#include "../PlayingState/FightManager.h"

class HudElements
{
private:
	static int wWidth;
	static int wHeight;
	static int dist;
	static int offset;

	static SDLUtils* sdl;
	static string fontstring;
	static Font* font;
	static string fontstringp;
	static Font* fontp;

	short r = 0;
	short g = 255;

	bool init = false;
public:

	int x = 0;
	int y = 0;

	Texture percent;
	Texture pNumber;
	Texture lives;

	SDL_Rect portraitRect = SDL_Rect();

	HudElements(FightManager* man)
	{
		if (!init)
		{
			HudElements::wWidth = man->GetActualWidth();
			HudElements::wHeight = man->GetActualHeight();

			HudElements::dist = wWidth / man->GetNumOfPlayers();
			HudElements::offset = (wWidth / 2) / man->GetNumOfPlayers() - wWidth / 30;

			HudElements::sdl = man->GetSDLU();
			HudElements::fontstring = "nes" + to_string(7 * wWidth / sdl->width());
			HudElements::font = &sdl->fonts().at(fontstring);
			HudElements::fontstringp = "nes" + to_string(10 * wWidth / sdl->width());
			HudElements::fontp = &sdl->fonts().at(fontstringp);

			init = true;
		}
	}

	void BootHUD(FightManager* man, const ushort& pos)
	{
		x = (int)(pos * HudElements::dist + HudElements::offset);
		y = HudElements::wHeight - (HudElements::wHeight / 6);

		portraitRect = { x, y, HudElements::wWidth / 14, HudElements::wWidth / 14 };

		string player = "Player " + to_string(pos + 1);
		SDL_Color c;
		if (pos == 0) c = build_sdlcolor(0xFF000000);
		else if (pos == 1) c = build_sdlcolor(0x002EFF00);
		else if (pos == 2) c = build_sdlcolor(0x00FF6100);
		else if (pos == 3) c = build_sdlcolor(0xFFF00000);
		else c = build_sdlcolor(0x00000000);
		pNumber = Texture(sdl->renderer(), player, *font, c);
		UpdateDmg(0);
		UpdateLives(3);
	}

	void UpdateLives(const ushort& liv)
	{
		string vidas = "Lives:" + to_string(liv);
		SDL_Color c;
		c = build_sdlcolor(0x00F7FF00);
		lives = Texture(sdl->renderer(), vidas, *font, c);
	}

	void UpdateDmg(const ushort& dmg)
	{
		string damage = to_string(dmg) + "%";
		if (dmg < 255)
		{
			if (r < 255) {
				r = dmg * 2;
				if (r > 255)r = 255;
			}
			else
			{
				g = 255 - dmg;
				if (g < 0)g = 0;
			}
		}
		else
		{
			Uint32 a = (Uint32)((((float)dmg - 255.f) / (float)SDL_MAX_SINT16) * 255 * 20);
			if (a < 255)
				r = 255 - a;
			else
				r = 0;
			g = 0;
		}
		Uint32 color = (Uint32)(r * pow(16, 6) + g * pow(16, 4));
		SDL_Color c = build_sdlcolor(color);
		percent = Texture(sdl->renderer(), damage, *fontp, c);
	}
};
