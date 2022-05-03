#include "../../Utils/AnimationManager.h"
#include "Spell.h"
#include "../../../utils/CheckML.h"

Spell::Spell(FightManager* manager, b2Vec2 pos, attackData attack, b2Vec2 dir) :
	Projectile(manager, pos, dir, 0.3f, 0.1f, 20)
{
	texture = &sdl->images().at("bullet");

	data = attack;

	duration = 200;

	body->GetUserData().pointer = 1; // Para atravesar el suelo
}

Spell::~Spell()
{

}