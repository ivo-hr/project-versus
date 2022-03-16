#include "InputConfig.h"



bool InputConfig::up()
{
	switch (typeofInput)
	{
	case 0:
		return ih.isKeyDown(SDLK_w);
		break;
	case 1:
		return ih.isKeyDown(SDLK_UP);
		break;
	case 2:
		return ih.getAxesState(0, 4) == -1;
		break;
	case 3:
		return ih.getAxesState(1, 4) == -1;
		break;
	}
}

bool InputConfig::down()
{

	switch (typeofInput)
	{
	case 0:
		return ih.isKeyDown(SDLK_s);
		break;
	case 1:

		return ih.isKeyDown(SDLK_DOWN);
		break;
	case 2:

		return ih.getAxesState(0, 4) == 1;
		break;
	case 3:
		return ih.getAxesState(1, 4) == 1;
		break;
	}
}

bool InputConfig::left()
{

	switch (typeofInput)
	{
	case 0:
		return ih.isKeyDown(SDLK_a);
		break;
	case 1:
		return ih.isKeyDown(SDLK_LEFT);
		break;
	case 2:
		return ih.getAxesState(0, 1) == -1;
		break;
	case 3:
		return ih.getAxesState(1, 1) == -1;
		break;
	}
}

bool InputConfig::right()
{

	switch (typeofInput)
	{
	case 0:
		return ih.isKeyDown(SDLK_d);
		break;
	case 1:
		return ih.isKeyDown(SDLK_RIGHT);
		break;
	case 2:
		return ih.getAxesState(0, 1) == 1;
		break;
	case 3:
		return ih.getAxesState(1, 1) == 1;
		break;
	}
}

bool InputConfig::basic()
{
	switch (typeofInput)
	{
	case 0:
		return ih.isKeyDown(SDLK_e);
		break;
	case 1:
		return ih.isKeyDown(SDLK_l);
		break;
	case 2:
		return ih.getButtonState(0, 1);
		break;
	case 3:
		return ih.getButtonState(1, 1);
		break;
	}
}

bool InputConfig::special()
{
	switch (typeofInput)
	{
	case 0:
		return ih.isKeyDown(SDLK_r);
		break;
	case 1:
		return ih.isKeyDown(SDLK_k);
		break;
	case 2:
		return ih.getButtonState(0, 2);
		break;
	case 3:
		return ih.getButtonState(1, 2);
		break;
	}
}

bool InputConfig::stop()
{

	switch (typeofInput)
	{
	case 0:
		return ih.isKeyUp(SDLK_a) && ih.isKeyUp(SDLK_d);
		break;
	case 1:
		return ih.isKeyUp(SDLK_LEFT) && ih.isKeyUp(SDLK_RIGHT);
		break;
	case 2:
		return ih.getAxesState(0, 1) == 0;
		break;
	case 3:
		return ih.getAxesState(1, 1) == 0;
		break;
	}
}
