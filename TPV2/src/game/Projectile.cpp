#include "Projectile.h"
#include "Utils/Particle.h"

Projectile::Projectile(FightManager* manager, Vector2D* pos, b2Vec2 dir, float width, float height, int speed) :
	Entity(manager, pos, width, height)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	iniPos = pos;

	vecDir = dir;

	vecDir.Normalize();

	if (vecDir.x < 0) {
		this->dir = -1;
	}

	ang = (acos(-vecDir.x) * 180) / M_PI;

	vecDir *= speed;
	
	body->SetGravityScale(0);
}

Projectile::~Projectile()
{

}

void Projectile::update()
{
	float distance = abs(body->GetPosition().x - iniPos->getX());
	if (distance <= range)
	{
		body->SetLinearVelocity(vecDir);

		Entity::update();
	}
	else
	{
		manager->RemoveEntity(this);
	}


}


void Projectile::draw()
{	
	texture->render(hurtbox, ang);
}

void Projectile::CheckHits()
{
	for (int j = 0; j < oponents.size(); j++)
	{
		SDL_Rect hitArea;
		if (SDL_IntersectRect(&hurtbox, oponents[j]->GetHurtbox(), &hitArea))
		{
			//Le hace da�o xddd
			if (oponents[j]->GetHit(data, this))
			{
				manager->HitLag(lag);

				oponents[j]->AddParticle(new Particle(
					new Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
					1, "sHitParticle", nullptr, oponents[j]));
			}
			manager->RemoveEntity(this);
		}
	}
}


