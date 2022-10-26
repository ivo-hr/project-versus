#include "Button.h"
#include "../../utils/CheckML.h"

void Button::render()
{
	rendered = true;
	if (!activated && destex != nullptr) {
		destex->render({ x,y,w,h });
	}
	else
	{
		tex->render({ x,y,w,h });
	}
}

void Button::render(const SDL_Rect& recorte)
{
	rendered = true;
	if (!activated && destex != nullptr) {
		destex->render(recorte, { x,y,w,h });
	}
	else
	{
		tex->render(recorte, { x,y,w,h });
	}
}

bool Button::mouseClick()
{
	if (rendered) {
		int mx = ih.getMousePos().first;
		int my = ih.getMousePos().second;

		if ((mx > x - (w / 20)) && (mx < (x - (w / 20)) + (w + (w / 10))) && (my > y - (h / 20)) && (my < (y - (h / 20)) + (h + h / 10)))
		{
			if (!ih.mouseButtonEvent())
			{
				pressed = false;
				return false;
			}
			if (ih.getMouseButtonState(ih.LEFT) && !pressed) {
				pressed = true;
				rendered = false;
				return true;
			}
			
		}
	}
	return false;
}

SDL_Rect Button::getRect()
{
	SDL_Rect r = { x, y, w, h };
	return r;
}

bool Button::pointerClick(SDL_Rect rect)
{
	if (rendered) {
		SDL_Rect r = build_sdlrect((float)x - (float)w / 20.f, (float)y - (float)h / 20.f, w + (float)w / 10.f, h + (float)h / 10.f);
		return SDL_HasIntersection(&r, &rect);
		rendered = false;
	}
	return false;
}

PlayButton::PlayButton(Texture* t, int x, int y, int width, int height) :Button(t, x, y, width, height)
{

	for (auto i = 0u; i < 22; i++) {
		std::string key = "C" + std::to_string(i);
		std::string file = "resources/images/rompe_cristal/" + std::to_string(i) + ".png";
		sdl->images().emplace(key, Texture(sdl->renderer(), file));
		txV.push_back(&sdl->images().at(key));
	}
}

void PlayButton::render()
{
	rendered = true;
	if (frame < 21) {
		if (cont + 5u < SDL_GetTicks()) {
			cont = SDL_GetTicks();
			frame++;
		}
	}
	txV[frame]->render({ x, y, w, h });

}
