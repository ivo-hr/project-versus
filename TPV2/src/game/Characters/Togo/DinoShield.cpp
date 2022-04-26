
#include "../../Utils/AnimationManager.h"	﻿
#include "Togo.h"
#include "DinoShield.h"
#include"../../PlayingState/FightManager.h"

DinoShield::DinoShield(FightManager* manager, Vector2D* pos) :
	Entity(manager, pos, 7.5, 5)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	texture = &sdl->images().at("dinoSouls");
}

DinoShield::~DinoShield()
{

}

void DinoShield::update()
{
}


void DinoShield::draw()
{
	texture->render(hurtbox);
}

void DinoShield::draw(SDL_Rect* camera)
{
	SDL_Rect aux = hurtbox;

	//si hurtbox.x = camera w + camera x                   aux.x = manager->GetActualWidth()
	//   hurtbox.x = camera w / 2 + camera x               aux.x = manager->GetActualWidth() / 2

	aux.x -= camera->x;
	aux.x *= (manager->GetActualWidth() / (float)camera->w);

	aux.y -= camera->y;
	aux.y *= (manager->GetActualHeight() / (float)camera->h);

	aux.w *= (manager->GetActualWidth() / (float)camera->w);
	aux.h *= (manager->GetActualHeight() / (float)camera->h);

	SDL_Rect src = { 896 - spriteX, 903 + spriteY, 128, 82};

	if (dir < 0)
		texture->render(src, aux);
	else
		texture->render(src, aux, 0, nullptr, SDL_FLIP_HORIZONTAL);

#ifdef _DEBUG

	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG

	if (anim >= 1) {
		if (spriteX == 0) {
			spriteX = 896;
			spriteY = 82;
		}
		else {
			spriteX = 0;
			spriteY = 0;
		}
		anim = 0;
	}
	anim += 0.1;

}

bool DinoShield::GetHit(attackData a, Entity* attacker)
{
	if (attacker->changeDir())
		attacker->SetOponents(oponents);
	return false;
}
