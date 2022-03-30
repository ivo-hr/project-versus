#pragma once

#include "../../Projectile.h"


class Spear : public Projectile
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
	Spear(FightManager* manager, Vector2D* pos, attackData attack, b2Vec2 dir, Togo* togo);
	~Spear();

	virtual void update() override;
	virtual void CheckHits() override;
	virtual bool GetHit(attackData a, Entity* attacker) { return(false);};
	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};

private:
	Togo* owner;
};