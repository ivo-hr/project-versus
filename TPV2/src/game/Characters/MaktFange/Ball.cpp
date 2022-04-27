#include "Ball.h"
#include "../../Utils/Particle.h"

MaktBall::MaktBall(FightManager* manager, Vector2D* pos, attackData attack, b2Vec2 dir, Vector2D* respawn) :
	Projectile(manager, pos, dir, 2.f, 2.f, 20)
{
	texture = &sdl->images().at("makt");

	vecDir = dir;

	vecDir.Normalize();

	vecDir *= attack.damage * 1.1f;

	range = attack.damage * 1.2f;

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
			float distance = abs(body->GetPosition().x - iniPos->getX());
			if (distance <= range)
			{
				body->SetLinearVelocity(vecDir);
			}
			else
			{
				physic = true;
				body->SetGravityScale(10.f);
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
						new Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
						1, "bHitParticle", oponents[j]));

					physic = true;
					body->SetGravityScale(10.f);
				}
			}
		}
	}
}

void MaktBall::OnDeath()
{
	body->SetTransform({ respawnPos.getX(), respawnPos.getY() }, 0);
	body->SetAwake(false);
	alive = false;
	physic = true;
	body->SetGravityScale(10.f);
}

bool MaktBall::PickUp()
{
	if (physic)
	{
		manager->RemoveEntity(this);
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
