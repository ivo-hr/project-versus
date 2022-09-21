#include "Projectile.h"
#include "Utils/Particle.h"
#include "../utils/CheckML.h"

Projectile::Projectile(FightManager* manager, b2Vec2 pos, b2Vec2 dir, float width, float height, int speed) :
	Entity(manager, pos, width, height)
{
	reflected = 0;

	vecDir = dir;

	vecDir.Normalize();

	if (vecDir.x < 0) {
		this->dir = -1;
	}

	ang = (acos(-vecDir.x) * 180) / M_PI;

	vecDir *= speed;

	body->SetGravityScale(0);

	projectile = true;

	outFor = 0;
}

Projectile::~Projectile()
{

}

void Projectile::update()
{
	outFor++;
	if (outFor <= duration)
	{
		body->SetLinearVelocity(vecDir);

		Entity::update();
	}
	else
	{
		toDelete = true;
	}
}


void Projectile::draw()
{	
	texture->render(hurtbox, ang);
}

void Projectile::draw(SDL_Rect* camera)
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

	texture->render(aux, ang);

#ifdef _DEBUG

	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG
}

void Projectile::CheckHits()
{
	if (reflected == 0)
	{
		Entity* oponent = nullptr;
		while (manager->GetNextEntity(oponent, layer))
		{
			SDL_Rect hitArea;
			if (SDL_IntersectRect(&hurtbox, oponent->GetHurtbox(), &hitArea))
			{
				bool controlHitLag = false;
				bool controlShake = false;
				bool controlCamShake = false;

				if (oponent->GetHit(data, this, controlHitLag, controlShake, controlCamShake))
				{
					if (!controlHitLag)
					{
						oponent->AddHitLag(lag);
						AddHitLag(lag);
					}

					oponent->AddParticle(new Particle(
						Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
						1, "sHitParticle", oponent));
				}
				if (reflected == 0)
					toDelete = true;
			}
		}
	}
	else
		reflected--;
}
bool Projectile::changeDir()
{
	outFor = 0;
	vecDir = -vecDir;
	data.damage *= 1.2f;
	reflected = 3;
	dir = -dir;
	return true;
}

