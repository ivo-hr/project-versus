#pragma once

#include "../../Entity.h"


class Explosion : public Entity
{
protected:
	HitData data;
public:
	Explosion(FightManager* manager, b2Vec2 pos, int power, int type, bool lookRight);
	virtual ~Explosion();

	virtual void update() override;
	virtual void draw() override;
	virtual void draw(SDL_Rect* camera) override;
	virtual void CheckHits() override;
	virtual SDL_Rect* GetHurtbox() { return &hurtbox; };
private:
	int anim = 0;
	int spDur = 0;

	int duration = 20;
	int time = 0;

	int spriteX = 0;
	int spriteY = 0;
};