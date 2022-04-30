#include "Button.h"

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

bool Button::mouseClick()
{
	if (rendered) {
		int mx = ih.getMousePos().first;
		int my = ih.getMousePos().second;

		if ((mx > x) && (mx < x + w) && (my > y) && (my < y + h))
		{
			if (ih.getMouseButtonState(ih.LEFT)) {
				pressed = true;
				rendered = false;
			}
		}
		if (!ih.mouseButtonEvent() && pressed) {
			pressed = false;
			rendered = false;
			return true;
		}
		return false;
	}
	return false;
}

SDL_Rect Button::getRect()
{
	SDL_Rect r = build_sdlrect(x, y, w, h);
	return r;
}

bool Button::pointerClick(SDL_Rect rect)
{
	if (rendered) {
		SDL_Rect r = build_sdlrect(x, y, w, h);
		return SDL_HasIntersection(&r, &rect);
		rendered = false;
	}
	return false;
}

PlayButton::PlayButton(Texture* t, int x, int y, int width, int height) :Button(t, x, y, width, height)
{
	for (auto i = 0u; i < 22; i++) {
		std::string c = "C" + std::to_string(i);
		txV.push_back(&sdl->images().at(c));
	}
}

void PlayButton::render()
{
	rendered = true;
	if (frame < 21) {
		if (cont + 5 < SDL_GetTicks()) {
			cont = SDL_GetTicks();
			frame++;
		}
	}
	txV[frame]->render({ x, y, w, h });

}
