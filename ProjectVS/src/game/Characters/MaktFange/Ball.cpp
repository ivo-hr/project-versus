#include "Ball.h"
#include "../../Utils/Particle.h"
#include "../../../utils/CheckML.h"

MaktBall::MaktBall(FightManager* manager, b2Vec2 pos, const HitData& attack, b2Vec2 dir, b2Vec2 respawn, ushort pNumber, ushort layer) :
	Projectile(manager, pos, dir, 1.5f, 1.5f, 20)
{
	arrowsTex = &sdl->images().at("arrows");
	arrowSrc = { 0, 0, arrowsTex->width(), arrowsTex->height() / 4 };

	texture = &sdl->images().at("makt");

	vecDir = dir;

	vecDir.Normalize();

	vecDir *= attack.damage * 0.8f;

	duration = (int)(attack.damage * 1.5f);

	physic = false;

	lag = 15;

	respawnPos = respawn;

	data = attack;

	respawnFrames = 180;

	manager->FollowCamera(this);

	AddTag(Tags::Hitable);

	alive = true;

	originalLayer = layer;
}

MaktBall::~MaktBall()
{
}

void MaktBall::update()
{
	if (hitLag > 0)
	{
		hitLag--;
		if (hitLag == 0)
		{
			body->SetEnabled(true);
		}
		return;
	}

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

				data.damage = (ushort)(data.damage * 0.75f);
				data.base = (ushort)(data.base * 0.5f);
				lag = 10;
			}
		}

		Entity::update();
	}
	else
	{
		respawnFrames--;
		if (respawnFrames == 0)
		{
			Respawn();
			respawnFrames = 180;
		}
	}
}

void MaktBall::CheckHits()
{
	if (body->GetLinearVelocity().Length() > 2.f && alive)
	{
		Entity* oponent = nullptr;
		while (manager->GetNextEntity(oponent, layer) && !isHit[oponent])
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

					if (!controlShake)
					{
						oponent->AddParticle("bigHit",
							Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
							1, false);
					}

					physic = true;
					body->SetGravityScale(10.f);
					body->SetLinearDamping(3.f);

					isHit[oponent] = true;
				}
			}
		}
	}
}

void MaktBall::SetOriginalLayer()
{
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
	if (physic && alive)
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

	manager->ChangeEntityLayer(this, originalLayer);
}

void MaktBall::draw(SDL_Rect* camera)
{
	if (alive)
	{
		SDL_Rect aux = hurtbox;

		float wDiff = (float)manager->GetActualWidth() / (float)camera->w;
		float hDiff = (float)manager->GetActualHeight() / (float)camera->h;

		aux.x -= camera->x;
		aux.x = (int)((float)aux.x * wDiff);

		aux.y -= camera->y;
		aux.y = (int)((float)aux.y * hDiff);

		aux.w = (int)((float)aux.w * wDiff);
		aux.h = (int)((float)aux.h * hDiff);

		SDL_Rect src = { 1260 + sprite, 1543, 17, 17 };
		if (dir < 0)
			texture->render(src, aux, ang, nullptr, SDL_FLIP_HORIZONTAL);
		else
			texture->render(src, aux);

		int xpos = aux.x + (aux.w / 2);

		arrowSrc.y = arrowSrc.h * (GetLayer() - 1);
		arrowsTex->render(arrowSrc, { xpos - 15, aux.y - 34, 30, 16 });

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
}
