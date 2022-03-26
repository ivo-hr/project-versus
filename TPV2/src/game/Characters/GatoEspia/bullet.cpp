#include "../../Utils/AnimationManager.h"
#include "bullet.h"

Bullet::Bullet(FightManager* manager, Vector2D* pos, attackData attack, int dir) :
	Entity(manager, pos, 0.3f, 0.1f)
{
	hurtbox = manager->GetSDLCoors(body, width, height);
	texture = &sdl->images().at("bullet");
	iniPos = position = pos;

	this->dir = dir;

	speed = 20 * dir;

	body->SetGravityScale(0);

	data = attack;
}

Bullet::~Bullet()
{

}

void Bullet::update()
{
	if (abs(position->getX() - iniPos->getX()) <= range)
	{
		body->SetLinearVelocity(b2Vec2(speed, 0));

		Entity::update();
	}
	else
	{
		manager->RemoveEntity(this);
	}


}


void Bullet::draw()
{
	texture->render(hurtbox);
}

void Bullet::CheckHits()
{
	for (int j = 0; j < oponents.size(); j++)
	{
		if (SDL_HasIntersection(&hurtbox, oponents[j]->GetHurtbox()))
		{
			//Le hace daño xddd
			if (oponents[j]->GetHit(data, dir))
			{
				manager->HitLag(3);
			}
			manager->RemoveEntity(this);
		}
	}
}


