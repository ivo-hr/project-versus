#pragma once

#include "../../Entity.h"


class Bullet : public Entity
{
protected:

	int speed;
	int damage;
	int range;
	Vector2D* position;
	Vector2D* iniPos;
public:
	Bullet(FightManager* manager, Vector2D* pos, float w, float h );
	~Bullet();

	virtual void update() override;
	virtual void draw() override;
	virtual bool GetHit(attackData a, int dir) { return(false);};
	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};
};