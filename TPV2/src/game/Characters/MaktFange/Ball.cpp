#include "Ball.h"
#include "../../Utils/Particle.h"
#include "../../../utils/CheckML.h"

MaktBall::MaktBall(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, b2Vec2 respawn) :
	Projectile(manager, pos, dir, 1.5f, 1.5f, 20)
{
	texture = &sdl->images().at("makt");

	vecDir = dir;

	vecDir.Normalize();

	vecDir *= attack.damage * 1.1f;

	duration = attack.damage * 2.2f;

	physic = false;

	lag = 15;

	respawnPos = respawn;

	data = attack;

	//respawnFrames = 180;
}

MaktBall::~MaktBall()
{
}

void MaktBall::update()
{
	if (alive)
	{
		if (!physic)
		{
			outFor++;
			if (outFor <= duration)
			{
				body->SetLinearVelocity(vecDir);

			}
			else
			{
				physic = true;
				body->SetGravityScale(10.f);

				body->SetLinearDamping(3.);
			}
		}

		Entity::update();
	}
	else
	{
		respawnFrames--;
		if (respawnFrames <= 0)
		{
			Respawn();
			respawnFrames = 180;
		}
	}
}

void MaktBall::CheckHits()
{
	if (!physic)
	{
		for (int j = 0; j < oponents.size(); j++)
		{
			SDL_Rect hitArea;
			if (SDL_IntersectRect(&hurtbox, oponents[j]->GetHurtbox(), &hitArea))
			{
				if (oponents[j]->GetHit(data, this))
				{
					manager->HitLag(lag);

					oponents[j]->AddParticle(new Particle(
						Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
						1, "bHitParticle", oponents[j]));

					physic = true;
					body->SetGravityScale(10.f);
					body->SetLinearDamping(3.);
				}
			}
		}
	}
}

void MaktBall::OnDeath()
{
	body->SetTransform(respawnPos, 0);
	body->SetAwake(false);
	alive = false;
	physic = true;
	body->SetGravityScale(10.f);
}

bool MaktBall::PickUp()
{
	if (physic)
	{
		toDelete = true;
		return true;
	}
	else
		return false;
}

void MaktBall::Respawn()
{
	body->SetAwake(true);

	body->SetLinearVelocity({ 0, 0 });

	alive = true;

	respawnFrames = 180;
}

void MaktBall::draw(SDL_Rect* camera)
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

	SDL_Rect src = { 1260 + sprite, 1543, 17, 17 };
	if (dir < 0)
		texture->render(src, aux, ang, nullptr, SDL_FLIP_HORIZONTAL);
	else
		texture->render(src, aux);

#ifdef _DEBUG

	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG

	/*if (anim >= 1) {
		if (sprite != 320) {
			sprite += 140;
		}
		else {
			sprite = 0;
		}
		anim = 0;
	}
	anim += 0.1;*/
}
