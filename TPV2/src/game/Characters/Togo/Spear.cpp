#include "../../Utils/AnimationManager.h"
#include "Togo.h"
#include "Spear.h"

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

		Entity::update();
	}
	else
	{
		owner->SetSpear(true);
		manager->RemoveEntity(this);		
	}
}

void Spear::CheckHits()
{
	for (int j = 0; j < oponents.size(); j++)
	{
		if (SDL_HasIntersection(&hurtbox, oponents[j]->GetHurtbox()))
		{
			//Le hace dano xddd
			if (oponents[j]->GetHit(data, this))
			{
				manager->HitLag(lag);
			}
		}
	}
}


