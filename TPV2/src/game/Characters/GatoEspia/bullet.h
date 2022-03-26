#pragma once

#include "../../Entity.h"


class Bullet : public Entity
{
protected:

	int speed;
	int damage;
	int range = 20;
	Vector2D* position;
	Vector2D* iniPos;

	attackData data;

public:
	Bullet(FightManager* manager, Vector2D* pos, attackData attack, int dir);
	~Bullet();

	virtual void update() override;
	virtual void draw() override;
	virtual void CheckHits() override;
	virtual bool GetHit(attackData a, int dir) { return(false);};
	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};
};