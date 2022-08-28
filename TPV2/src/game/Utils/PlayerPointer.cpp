#include "PlayerPointer.h"
#include "../../utils/CheckML.h"

void PlayerPointer::render()
{	if(active)
	texture->render({ x,y,w,h });
}

void PlayerPointer::move(int dir)
{
	//SDL_GetWindowSize(sdl->window(), &ww, &wh);
	vel = ((float)ww / 200.f) + 1;

	if (active)
	switch (dir)
	{
	case 0:
		if (y > 0)
		y = y - vel;
		break;
	case 1:
		if (y+h < wh)
		y = y + vel;
		break;
	case 2:
		if (x > 0)
		x = x - vel;
		break;
	case 3:
		if (x+w < ww)
		x = x + vel;
		break;
	default:
		break;
	}
}

SDL_Rect PlayerPointer::getRect()
{
	SDL_Rect r = build_sdlrect(x, y, w, h);
	return r;
}

void PlayerPointer::setPosition(int newX, int newY)
{
	x = newX;
	y = newY;
}
