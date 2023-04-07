#include "Projectile.h"
#include "Utils/Particle.h"
#include "../utils/CheckML.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"

Projectile::Projectile(FightManager* manager, b2Vec2 pos, b2Vec2 dir, float width, float height, int speed) :
	Entity(manager, pos, width, height)
{
	reflected = 0;

	vecDir = dir;

	vecDir.Normalize();

	if (vecDir.x < 0) {
		this->dir = -1;
	}

	ang = (acos(-vecDir.x) * 180) / (float)M_PI;

	vecDir.x = vecDir.x * (float)speed;
	vecDir.y = vecDir.y * (float)speed;

	body->SetGravityScale(0);

	AddTag(Tags::IsProjectile);

	outFor = 0;
}

Projectile::~Projectile()
{

}

void Projectile::update()
{
	if (!alive)
		return;

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
	texture->render(hurtbox, 1);
}

void Projectile::draw(const SDL_Rect& camera)
{
	SDL_Rect aux = hurtbox;

	float wDiff = (float)manager->GetActualWidth() / (float)camera.w;
	float hDiff = (float)manager->GetActualHeight() / (float)camera.h;

	aux.x -= camera.x;
	aux.x = (int)((float)aux.x * wDiff);

	aux.y -= camera.y;
	aux.y = (int)((float)aux.y * hDiff);

	aux.w = (int)((float)aux.w * wDiff);
	aux.h = (int)((float)aux.h * hDiff);

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
			if (SDL_IntersectRect(&hurtbox, &oponent->GetHurtbox(), &hitArea))
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

					if (!controlShake)
					{
						oponent->AddParticle("smallHit",
							Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
							1, false);
					}

					if (reflected == 0)
						toDelete = true;
				}
			}
		}
	}
	else
		reflected--;
}
bool Projectile::changeDir()
{
	outFor = 0; 
	duration -= 10;
	vecDir = -vecDir;
	data.damage = (ushort)((float)data.damage * 1.2f);
	reflected = 3;
	dir = -dir;
	return true;
}

