#include "../../Utils/AnimationManager.h"
#include "bullet.h"

Bullet::Bullet(FightManager* manager, Vector2D* pos,float w, float h) :
	Entity(manager, pos, w, h)
{
	hurtbox = manager->GetSDLCoors(body, width, height);
	texture = &sdl->images().at("bullet");
	iniPos = position = pos;
}

Bullet::~Bullet()
{

}

void Bullet::update()
{
	if ((position->getX() - iniPos->getX()) >= 20)
	{
		body->SetLinearVelocity(b2Vec2(0, 5));
	}

	Entity::update();


}


void Bullet::draw()
{
	anim->render();

}
