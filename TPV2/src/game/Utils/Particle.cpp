#include "Particle.h"
#include "../../utils/CheckML.h"

void Particle::CalcularResto()
{

	spriteDuration = data.duration / data.numSprites;
	cont = spriteDuration;

	src.x = 0;
	src.y = 0;

	src.w = texture->width() / data.numSpritesinX;
	src.h = texture->height() / data.numSpritesinY;
}

Particle::Particle(const Vector2D& position, const std::string& id, Entity* ent, int dir)
{
	data.dir = dir;
	manager = ent->GetManager();
	active = false;

	if (id == "sHitParticle")
	{
		int w = 75;
		int h = 75;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &manager->GetSDLU()->images().at("sHitParticle");

		data.numSpritesinX = 2;
		data.numSpritesinY = 3;
		data.numSprites = 5;
		data.duration = 15;
	}
	else if (id == "bHitParticle")
	{
		int w = 120;
		int h = 120;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &manager->GetSDLU()->images().at("bHitParticle");

		data.numSpritesinX = 2;
		data.numSpritesinY = 4;
		data.numSprites = 7;
		data.duration = 35;
	}
	else if (id == "run")
	{
		int w = 50;
		int h = w / 2;

		if (dir > 0)
		{
			data.dest = { (int)(position.getX() - w), (int)(position.getY() - h), (int)w, (int)h };
		}
		else
		{
			data.dest = { (int)(position.getX()), (int)(position.getY() - h), (int)w, (int)h };
		}

		texture = &manager->GetSDLU()->images().at("run");

		data.numSpritesinX = 1;
		data.numSpritesinY = 3;
		data.numSprites = 3;
		data.duration = 12;
	}
	else if (id == "killHit")
	{
		int w = 620;
		int h = w / 4;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &manager->GetSDLU()->images().at("killHit");

		data.numSpritesinX = 1;
		data.numSpritesinY = 6;
		data.numSprites = 6;
		data.duration = 40;
	}
	else if (id == "killVfx")
	{
		int w = 384;
		int h = w;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &manager->GetSDLU()->images().at("killVfx");

		data.numSpritesinX = 3;
		data.numSpritesinY = 3;
		data.numSprites = 7;
		data.duration = 30;
	}
	else if (id == "died")
	{
		int w = 1024;
		int h = w;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h};

		texture = &manager->GetSDLU()->images().at("died");

		data.numSpritesinX = 3;
		data.numSpritesinY = 3;
		data.numSprites = 7;
		data.duration = 40;
	}
	else if (id == "water")
	{
		int w = 32;
		int h = w;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &manager->GetSDLU()->images().at("water");

		data.numSpritesinX = 2;
		data.numSpritesinY = 2;
		data.numSprites = 4;
		data.duration = 30;
	}
	else if (id == "electric")
	{
		int w = 32;
		int h = w;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &manager->GetSDLU()->images().at("electric");

		data.numSpritesinX = 2;
		data.numSpritesinY = 2;
		data.numSprites = 4;
		data.duration = 30;
	}
	else if (id == "fire")
	{
		int w = 32;
		int h = w;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &manager->GetSDLU()->images().at("fire");

		data.numSpritesinX = 2;
		data.numSpritesinY = 2;
		data.numSprites = 3;
		data.duration = 30;
	}
	else if (id == "shieldBroken")
	{
		int w = 128;
		int h = w;

		data.dest = { (int)(position.getX() - w / 2), (int)(position.getY() - h / 2), (int)w, (int)h };

		texture = &manager->GetSDLU()->images().at("shieldBroken");

		data.numSpritesinX = 1;
		data.numSpritesinY = 6;
		data.numSprites = 6;
		data.duration = 30;
	}
	else
	{
		int w = 0;
		int h = 0;

		data.dest = { 0, 0, 0, 0 };

		texture = &manager->GetSDLU()->images().at("killVfx");
		data.numSpritesinX = 0;
		data.numSpritesinY = 0;
		data.numSprites = 1;
		data.duration = 0;
	}

	CalcularResto();
}

Particle::Particle(const Vector2D& position, const ParticleData& data, Entity* ent)
{
	manager = ent->GetManager();
	this->data = data;
	active = false;
	CalcularResto();
}

Particle::~Particle()
{
}

void Particle::Activate(const Vector2D& pos, short dir, bool isRun)
{
	active = true;

	if (!isRun)
		data.dest = { (int)(pos.getX() - data.dest.w / 2), (int)(pos.getY() - data.dest.h / 2), (int)data.dest.w, (int)data.dest.h };
	else
	{
		if (dir > 0)
		{
			data.dest = { (int)(pos.getX() - data.dest.w), (int)(pos.getY() - data.dest.h), (int)data.dest.w, (int)data.dest.h };
		}
		else
		{
			data.dest = { (int)(pos.getX()), (int)(pos.getY() - data.dest.h), (int)data.dest.w, (int)data.dest.h };
		}
	}
	data.dir = dir;
}

void Particle::Deactivate()
{
	active = false;
	cont = spriteDuration;

	src.x = 0;
	src.y = 0;
}

void Particle::update()
{
	if (active)
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
					Deactivate();
				}
			}
		}
	}
}

void Particle::draw()
{
	if (data.dir < 0)
	{
		texture->render(src, data.dest, 0, nullptr, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		texture->render(src, data.dest);
	}
}

void Particle::draw(SDL_Rect* camera)
{
	if (active)
	{
		SDL_Rect aux = data.dest;

		aux.x -= camera->x;
		aux.x *= (manager->GetActualWidth() / (float)camera->w);

		aux.y -= camera->y;
		aux.y *= (manager->GetActualWidth() / (float)camera->w);

		aux.w *= (manager->GetActualWidth() / (float)camera->w);
		aux.h *= (manager->GetActualWidth() / (float)camera->w);

		if (data.dir < 0)
		{
			texture->render(src, aux, 0, nullptr, SDL_FLIP_HORIZONTAL);
		}
		else
		{
			texture->render(src, aux);
		}
	}
}
