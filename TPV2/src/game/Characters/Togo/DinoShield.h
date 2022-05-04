#pragma once

#include "../../Entity.h"


class DinoShield : public Entity
{
protected:
	attackData data;
public:
	DinoShield(FightManager* manager, b2Vec2 pos);
	virtual ~DinoShield();

	virtual void update() override;
	virtual void draw() override;
	virtual void draw(SDL_Rect* camera) override;
	virtual void CheckHits() override {};
	virtual bool GetHit(attackData a, Entity* attacker);
	virtual SDL_Rect* GetHurtbox() { return &hurtbox; };
	virtual void setToDelete() { toDelete = true; }
private:
	int spriteX = 896;
	int spriteY = 82;
	float anim = 0;
};