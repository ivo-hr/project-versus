#include "Button.h"

void Button::render()
{
	rendered = true;
	if (pressed && presstex != nullptr) {
		presstex->render({ x,y,w,h });
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
			}
		}
		if (!ih.mouseButtonEvent() && pressed) {
			pressed = false;
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
