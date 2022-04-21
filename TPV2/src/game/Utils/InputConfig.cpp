#include "InputConfig.h"



// valor de typeofInput (al menos de momento):
// 0 y 1: teclado
// 2 y 3: mando NES
// 4 y 5: mando PS4 o Xbox One

bool InputConfig::up()
{
	if (typeofInput == -1) {
		return ih.isKeyDown(SDLK_w);
	}
	else if (typeofInput == -2) {
		return ih.isKeyDown(SDLK_UP);
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 1) == -1;
	}
}

bool InputConfig::down()
{
	if (typeofInput == -1) {
		return ih.isKeyDown(SDLK_s);
	}
	else if (typeofInput == -2) {
		return ih.isKeyDown(SDLK_DOWN);
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 1) == 1;
	}
}

bool InputConfig::downReleased()
{
	if (typeofInput == -1) {
		return ih.isKeyUp(SDLK_s);
	}
	else if (typeofInput == -2) {
		return ih.isKeyUp(SDLK_DOWN);
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 1) == 0;
	}
}

bool InputConfig::left()
{
	if (typeofInput == -1) {
		return ih.isKeyDown(SDLK_a);
	}
	else if (typeofInput == -2) {
		return ih.isKeyDown(SDLK_LEFT);
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 0) == -1;
	}
	
	
}

bool InputConfig::right()
{
	if (typeofInput == -1) {
		return ih.isKeyDown(SDLK_d);
	}
	else if (typeofInput == -2) {
		return ih.isKeyDown(SDLK_RIGHT);
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 0) == 1;
	}
}

bool InputConfig::basic()
{
	if (typeofInput == -1) {
		return ih.isKeyDown(SDLK_e);
	}
	else if (typeofInput == -2) {
		return ih.isKeyDown(SDLK_l);
	}
	else {
		return ih.getButtonState(typeofInput, 1);
	}
}

bool InputConfig::special()
{
	if (typeofInput == -1) {
		return ih.isKeyDown(SDLK_r);
	}
	else if (typeofInput == -2) {
		return ih.isKeyDown(SDLK_k);
	}
	else {
		return ih.getButtonState(typeofInput, 2);
	}
}

bool InputConfig::stop()
{
	if (typeofInput == -1) {
		return ih.isKeyUp(SDLK_a) && ih.isKeyUp(SDLK_d);
	}
	else if (typeofInput == -2) {
		return ih.isKeyUp(SDLK_LEFT) && ih.isKeyUp(SDLK_RIGHT);
	}
	else {
		return  ih.xboxGetAxesState(typeofInput, 0) == 0;
	}
}
