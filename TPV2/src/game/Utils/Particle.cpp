#include "Particle.h"

void Particle::CalcularResto()
{

	spriteDuration = duration / numSprites;
	cont = spriteDuration;

	src.x = 0;
	src.y = 0;

	src.w = texture->width() / numSpritesinX;
	src.h = texture->height() / numSpritesinY;
}

Particle::Particle(Vector2D position, int dir, std::string id, FightManager* mngr, Entity* ent): manager(mngr), ent(ent), dir(dir)
{

	if (mngr == nullptr)
	{
		mngr = ent->GetManager();
	}

	if (id == "sHitParticle")
	{
		int w = 75;
		int h = 75;

		dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &mngr->GetSDLU()->images().at("sHitParticle");

		numSpritesinX = 2;
		numSpritesinY = 3;
		numSprites = 5;
		duration = 15;
	}
	else if (id == "bHitParticle")
	{
		int w = 120;
		int h = 120;

		dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &mngr->GetSDLU()->images().at("bHitParticle");

		numSpritesinX = 2;
		numSpritesinY = 4;
		numSprites = 7;
		duration = 35;
	}
	else if (id == "run")
	{
		int w = 50;
		int h = w / 2;

		if (dir > 0)
		{
			dest = { (int)(position.getX() - w), (int)(position.getY() - h), (int)w, (int)h };
		}
		else
		{
			dest = { (int)(position.getX()), (int)(position.getY() - h), (int)w, (int)h };
		}

		texture = &mngr->GetSDLU()->images().at("run");

		numSpritesinX = 1;
		numSpritesinY = 3;
		numSprites = 3;
		duration = 12;
	}
	else if (id == "killHit")
	{
		int w = 620;
		int h = w / 4;

		dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &mngr->GetSDLU()->images().at("killHit");

		numSpritesinX = 1;
		numSpritesinY = 6;
		numSprites = 6;
		duration = 40;
	}
	else if (id == "killVfx")
	{
		int w = 384;
		int h = w;

		dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &mngr->GetSDLU()->images().at("killVfx");

		numSpritesinX = 3;
		numSpritesinY = 3;
		numSprites = 7;
		duration = 30;
	}

	CalcularResto();
}

Particle::~Particle()
{
}

void Particle::update()
{
	cont--;
	if (cont == 0)
	{
		cont = spriteDuration;

		if (src.x + src.w < texture->width())
			src.x += src.w;
		else
		{
			if (src.y + src.h < texture->height())
			{
				src.x = 0;
				src.y += src.h;
			}
			else
			{
				if (ent == nullptr)
				{
					manager->RemoveParticle(this);
				}
				else
				{
					ent->RemoveParticle(this);
				}
			}
		}
	}
}

void Particle::draw()
{
	if (dir < 0)
	{
		texture->render(src, dest, 0, nullptr, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		texture->render(src, dest);
	}
}