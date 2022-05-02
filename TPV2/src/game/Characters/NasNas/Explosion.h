#pragma once

#include "../../Entity.h"


class Explosion : public Entity
{
protected:
	attackData data;
public:
	Explosion(FightManager* manager, Vector2D* pos, int power, int type);
	~Explosion();

	virtual void update() override;
	virtual void draw() override;
	virtual void draw(SDL_Rect* camera) override;
	virtual void CheckHits() override;
	virtual bool GetHit(attackData a, Entity* attacker) { return (false); };
	virtual SDL_Rect* GetHurtbox() { return &hurtbox; };
private:
	int spriteX = 896;
	int spriteY = 82;
	float anim = 0;
};