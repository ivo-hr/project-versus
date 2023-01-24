#include "PlayerPointer.h"
#include "../../utils/CheckML.h"

void PlayerPointer::render()
{	
	if(active)
		texture->render({ x,y,w,h });
}

void PlayerPointer::update()
{
	if (input < -2 || !active)
		return;

	clickedLastFrame = clicked;
	clicked = false;

	switch (input)
	{
	case -1:
		if (ih.isKeyDown(playerPrefs.Keyboard1Up()))move(0);
		if (ih.isKeyDown(playerPrefs.Keyboard1Down()))move(1);
		if (ih.isKeyDown(playerPrefs.Keyboard1Left()))move(2);
		if (ih.isKeyDown(playerPrefs.Keyboard1Right()))move(3);
		if (ih.isKeyDown(playerPrefs.Keyboard1Basic()))clicked = true;
		break;
	case -2:
		if (ih.isKeyDown(playerPrefs.Keyboard2Up()))move(0);
		if (ih.isKeyDown(playerPrefs.Keyboard2Down()))move(1);
		if (ih.isKeyDown(playerPrefs.Keyboard2Left()))move(2);
		if (ih.isKeyDown(playerPrefs.Keyboard2Right()))move(3);
		if (ih.isKeyDown(playerPrefs.Keyboard2Basic()))clicked = true;
		break;
	default:
		if (ih.xboxGetAxesState(input, 1) == -1 || ih.xboxGetDpadState(input, 0))
			move(0);
		if (ih.xboxGetAxesState(input, 1) == 1 || ih.xboxGetDpadState(input, 2))
			move(1);
		if (ih.xboxGetAxesState(input, 0) == -1 || ih.xboxGetDpadState(input, 3))
			move(2);
		if (ih.xboxGetAxesState(input, 0) == 1 || ih.xboxGetDpadState(input, 1))
			move(3);
		if (ih.xboxGetButtonState(input, playerPrefs.ControllerBasic()))
			clicked = true;
		break;
	}
}

bool PlayerPointer::Click()
{
	return clicked && !clickedLastFrame;
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
