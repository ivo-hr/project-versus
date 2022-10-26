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
		y = (int)((float)y - vel);
		break;
	case 1:
		if (y+h < wh)
		y = (int)((float)y + vel);
		break;
	case 2:
		if (x > 0)
		x = (int)((float)x - vel);
		break;
	case 3:
		if (x+w < ww)
		x = (int)((float)x + vel);
		break;
	default:
		break;
	}
}

SDL_Rect PlayerPointer::getRect()
{
	SDL_Rect r = { x, y, w, h };
	return r;
}

void PlayerPointer::setPosition(int newX, int newY)
{
	x = newX;
	y = newY;
}
