#include "../../Utils/AnimationManager.h"
#include "bullet.h"
#include "../../../utils/CheckML.h"
#include "../../../sdlutils/SDLUtils.h"

Bullet::Bullet(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, float width, float height, int speed, std::string name) :
	Projectile(manager, pos, dir, width, height, speed)
{
	texture = &sdl->images().at(name);
	//funciona , but i dont know why
	data = attack;

	duration = 140;

	body->GetUserData().pointer = 1; // Para atravesar el suelo
}

Bullet::~Bullet()
{

}