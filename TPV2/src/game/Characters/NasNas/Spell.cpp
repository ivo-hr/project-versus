#include "../../Utils/AnimationManager.h"
#include "Spell.h"

Spell::Spell(FightManager* manager, Vector2D* pos, attackData attack, b2Vec2 dir) :
	Projectile(manager, pos, dir, 0.3f, 0.1f, 20)
{
	texture = &sdl->images().at("bullet");
	//funciona , but i dont know why
	data = attack;

	range = 20.f;

}

Spell::~Spell()
{

}