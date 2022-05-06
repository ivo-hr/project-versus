
#include "../../Utils/AnimationManager.h"	﻿
#include "Explosion.h"
#include"../../PlayingState/FightManager.h"
#include "../../../utils/CheckML.h"

Explosion::Explosion(FightManager* manager, b2Vec2 pos, int power, int type) :
	Entity(manager, pos, 7.5f, 5.f)
{

	//this->SetOponents(manager->GetEntities(this));
	hurtbox = manager->GetSDLCoors(body, width, height);
	data.direction = b2Vec2(1 , 1.8);
	//agua y fuego
	if (type == 0)
	{
		data.base = power / 2;
		data.damage = 10;
		data.multiplier = 0.4;
		texture = &sdl->images().at("ExplosionWF");
	}
	else{
		data.base = 10;
		data.damage = power / 2;
		data.multiplier = 0.4;
		texture = &sdl->images().at("ExplosionEF");
	}

	spDur = duration / 5;
}

Explosion::~Explosion()
{

}

void Explosion::update()
{
	time++;
	if (time >= duration)
	{
		toDelete = true;
	}

	anim++;
	if (anim == spDur) {
		
		spriteX += texture->width() / 2;

		if (spriteX == texture->width()) {

			spriteX = 0;
			spriteY += texture->height() / 3;

			if (spriteY == texture->height()) {
				toDelete = true;
			}
		}
		anim = 0;
	}
}
void Explosion::CheckHits()
{
	for (int j = 0; j < oponents.size(); j++)
	{
		SDL_Rect hitArea;
		if (SDL_IntersectRect(&hurtbox, oponents[j]->GetHurtbox(), &hitArea))
		{
			if (oponents[j]->GetHit(data, this))
			{		
				/*oponents[j]->AddParticle(new Particle(
				Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
				1, "sHitParticle", oponents[j]));*/
			}
		}
	}
}

void Explosion::draw()
{
	texture->render({ spriteX, spriteY, texture->width() / 2, texture->height() / 3 }, hurtbox);
}

void Explosion::draw(SDL_Rect* camera)
{
	SDL_Rect aux = { hurtbox.x, hurtbox.y - hurtbox.w / 4, hurtbox.w, hurtbox.w };

	//si hurtbox.x = camera w + camera x                   aux.x = manager->GetActualWidth()
	//   hurtbox.x = camera w / 2 + camera x               aux.x = manager->GetActualWidth() / 2

	aux.x -= camera->x;
	aux.x *= (manager->GetActualWidth() / (float)camera->w);

	aux.y -= camera->y;
	aux.y *= (manager->GetActualHeight() / (float)camera->h);

	aux.w *= (manager->GetActualWidth() / (float)camera->w);
	aux.h *= (manager->GetActualHeight() / (float)camera->h);

	SDL_Rect src = { spriteX, spriteY, texture->width() / 2, texture->height() / 3 };

	if (dir < 0)
		texture->render(src, aux);
	else
		texture->render(src, aux, 0, nullptr, SDL_FLIP_HORIZONTAL);

#ifdef _DEBUG

	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG

}
