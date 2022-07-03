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
	Spell(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, state state);
	virtual ~Spell();

	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};

	virtual void draw(SDL_Rect* camera) override;

	int sprite = 0;
	float anim = 0;
};