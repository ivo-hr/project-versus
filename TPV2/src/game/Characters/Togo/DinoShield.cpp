
#include "../../Utils/AnimationManager.h"	﻿
#include "Togo.h"
#include "DinoShield.h"
#include"../../PlayingState/FightManager.h"

DinoShield::DinoShield(FightManager* manager, Vector2D* pos) :
	Entity(manager, pos, 4, 4)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	texture = &sdl->images().at("run");

	SetOponents(oponents);
	manager->SetOpponents();

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

	aux.x -= camera->x;
	aux.x *= (manager->GetActualWidth() / (float)camera->w);

	aux.y -= camera->y;
	aux.y *= (manager->GetActualHeight() / (float)camera->h);

	aux.w *= (manager->GetActualWidth() / (float)camera->w);
	aux.h *= (manager->GetActualHeight() / (float)camera->h);

	texture->render(aux);

	SDL_RenderDrawRect(sdl->renderer(), &aux);

}


void DinoShield::CheckHits()
{

	for (int j = 0; j < oponents.size(); j++)
	{
		SDL_Rect hitArea;
		if (SDL_IntersectRect(&hurtbox, oponents[j]->GetHurtbox(), &hitArea))
		{
			oponents[j]->changeDir();   //Devuelve el disparo
		}
	}
}