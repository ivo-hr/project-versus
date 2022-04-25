#pragma once

#include "../../Projectile.h"


class Spell : public Projectile
{
//protected:
//
//	int speed;
//	int damage;
//	int range = 20;
//	Vector2D* position;
//	Vector2D* iniPos;
//	b2Vec2 vecDir;
//	attackData data;
//	float ang;

public:
	Spell(FightManager* manager, Vector2D* pos, attackData attack, b2Vec2 dir);
	~Spell();

	virtual bool GetHit(attackData a, Entity* attacker) { return(false);};
	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};
};