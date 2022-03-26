#include "../../Utils/AnimationManager.h"
#include "bullet.h"

Bullet::Bullet(FightManager* manager, Vector2D* pos, attackData attack, b2Vec2 dir) :
	Entity(manager, pos, 0.3f, 0.1f)
{
	hurtbox = manager->GetSDLCoors(body, width, height);
	texture = &sdl->images().at("bullet");
	iniPos = position = pos;

	vecDir = dir;

	speed = 20;


	vecDir.Normalize();

	//funciona , but i dont know why
	ang = (acos(-vecDir.x)*180)/M_PI ;
	vecDir *= speed;

	

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
		body->SetLinearVelocity(vecDir);

		Entity::update();
	}
	else
	{
		manager->RemoveEntity(this);
	}


}


void Bullet::draw()
{
	
	texture->render(hurtbox,ang);
}

void Bullet::CheckHits()
{
	for (int j = 0; j < oponents.size(); j++)
	{
		if (SDL_HasIntersection(&hurtbox, oponents[j]->GetHurtbox()))
		{
			//Le hace da�o xddd
			if (oponents[j]->GetHit(data, dir))
			{
				manager->HitLag(3);
			}
			manager->RemoveEntity(this);
		}
	}
}


