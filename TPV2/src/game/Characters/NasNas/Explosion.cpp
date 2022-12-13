#include "Explosion.h"
#include "../../PlayingState/FightManager.h"
#include "../../../utils/CheckML.h"

Explosion::Explosion(FightManager* manager, b2Vec2 pos, int power, int type, bool lookRight) :
	Entity(manager, pos, 7.5f, 5.f)
{

	//this->SetOponents(manager->GetEntities(this));
	hurtbox = manager->GetSDLCoors(body, width, height);
	data.direction = b2Vec2(1 , 1.8f);
	//agua y fuego
	if (type == 0)
	{
		data.base = power * 0.7f;
		data.damage = 10;
		data.multiplier = 0.9f;
		texture = &sdl->images().at("ExplosionWF");
		sdl->soundEffects().at("FWexplosion").play();
	}
	else
	{
		data.base = 40;
		data.damage = power;
		data.multiplier = 0.05f;
		texture = &sdl->images().at("ExplosionEF");
		sdl->soundEffects().at("FEexplosion").play();
	}

	dir = lookRight ? 1 : -1;

	spDur = duration / 5;

	this->type = type == 0;
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
	if (time >= 2)
	{
		Entity* oponent = nullptr;
		while (manager->GetNextEntity(oponent, 0))
		{
			SDL_Rect hitArea;
			if (SDL_IntersectRect(&hurtbox, oponent->GetHurtbox(), &hitArea) && !isHit[oponent])
			{
				manager->MoveToFront(this);
				bool controlHitLag = false;
				bool controlShake = false;
				bool controlCamShake = false;

				if (oponent->GetHit(data, this, controlHitLag, controlShake, controlCamShake))
				{
					if (type)
					{
						if (!controlHitLag)
						{
							oponent->AddHitLag(20);
							AddHitLag(20);
						}

						if (!controlShake)
						{
							oponent->SetShake(Vector2D(data.direction.x * 0.4f, data.direction.y * 0.4f), 10);
						}

						if (!controlCamShake)
						{
							manager->SetShake(Vector2D(data.direction.x * -3, data.direction.y * 6), 30);
						}
					}
					else
					{
						if (!controlHitLag)
						{
							oponent->AddHitLag(30);
							AddHitLag(30);
						}

						if (!controlShake)
						{
							oponent->SetShake(Vector2D(data.direction.x, data.direction.y), 15);
						}

						if (!controlCamShake)
						{
							manager->SetShake(Vector2D(data.direction.x * -6, data.direction.y * 9), 25);
						}
					}

					isHit[oponent] = true;
				}
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

	float wDiff = (float)manager->GetActualWidth() / (float)camera->w;
	float hDiff = (float)manager->GetActualHeight() / (float)camera->h;

	aux.x -= camera->x;
	aux.x = (int)((float)aux.x * wDiff);

	aux.y -= camera->y;
	aux.y = (int)((float)aux.y * hDiff);

	aux.w = (int)((float)aux.w * wDiff);
	aux.h = (int)((float)aux.h * hDiff);

	SDL_Rect src = { spriteX, spriteY, texture->width() / 2, texture->height() / 3 };

	if (dir < 0)
		texture->render(src, aux);
	else
		texture->render(src, aux, 0, nullptr, SDL_FLIP_HORIZONTAL);

#ifdef _DEBUG

	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG

}
