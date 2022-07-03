#pragma once

#include "../../Projectile.h"


class Bullet : public Projectile
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
	Bullet(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir);
	virtual ~Bullet();
	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};
};