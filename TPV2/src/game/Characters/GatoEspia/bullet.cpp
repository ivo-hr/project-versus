#include "../../Utils/AnimationManager.h"
#include "bullet.h"
#include "../../../utils/CheckML.h"

Bullet::Bullet(FightManager* manager, b2Vec2 pos, attackData attack, b2Vec2 dir) :
	Projectile(manager, pos, dir, 0.5f, 0.2f, 20)
{
	texture = &sdl->images().at("bullet");
	//funciona , but i dont know why
	data = attack;

	duration = 140;

	body->GetUserData().pointer = 1; // Para atravesar el suelo
}

Bullet::~Bullet()
{

}