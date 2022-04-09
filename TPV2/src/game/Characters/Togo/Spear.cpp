#include "../../Utils/AnimationManager.h"
#include "Togo.h"
#include "Spear.h"
#include "../../Utils/Particle.h"

Spear::Spear(FightManager* manager, Vector2D* pos, attackData attack, b2Vec2 dir, Togo* togo) :
	Projectile(manager, pos, dir, 2.f, 0.5f, 20)
{
	owner = togo;
	texture = &sdl->images().at("bullet");
	//funciona , but i dont know why
	data = attack;

	range = 30.f;

	lag = 1;
}

Spear::~Spear()
{

}

void Spear::update()
{
	float distance = abs(body->GetPosition().x - iniPos->getX());
	if (distance <= range)
	{
		body->SetLinearVelocity(vecDir);

		hurtbox.x = manager->b2ToSDLX(body, width);
		hurtbox.y = manager->b2ToSDLY(body, height);

		updateParticles();

		if (!SDL_HasIntersection(&hurtbox, manager->GetDeathZone()))
		{
			OnDeath();
		}
	}
	else
	{
		OnDeath();
	}
}

void Spear::CheckHits()
{
	for (int j = 0; j < oponents.size(); j++)
	{
		if (SDL_HasIntersection(&hurtbox, oponents[j]->GetHurtbox()))
		{
			SDL_Rect hitArea;
			if (SDL_IntersectRect(&hurtbox, oponents[j]->GetHurtbox(), &hitArea))
			{
				manager->HitLag(lag);
			}
		}
	}
}

void Spear::OnDeath() {
	owner->SetSpear(true);
	manager->RemoveEntity(this);
}


