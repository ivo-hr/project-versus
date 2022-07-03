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
	Spear(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, Togo* togo);
	virtual ~Spear();

	virtual void update() override;
	virtual void CheckHits() override;
	virtual bool GetHit(attackData a, Entity* attacker) { return(false);};
	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};

	virtual void OnDeath() override;

	virtual void draw(SDL_Rect* camera) override;

private:
	Togo* owner;
	
	int sprite = 0;
	float anim = 0;

	int hitDelay;
};