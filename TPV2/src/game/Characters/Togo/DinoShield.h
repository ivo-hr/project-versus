#pragma once

#include "../../Entity.h"


class DinoShield : public Entity
{
protected:
	attackData data;
public:
	DinoShield(FightManager* manager, Vector2D* pos);
	~DinoShield();

	virtual void update() override;
	virtual void draw() override;
	virtual void draw(SDL_Rect* camera) override;
	virtual void CheckHits() override;
	virtual bool GetHit(attackData a, Entity* attacker) { return false; };
	virtual SDL_Rect* GetHurtbox() { return &hurtbox; };
	void SetChangable();
private:
	std::vector<bool> changable;
	int spriteX = 896;
	int spriteY = 82;
	float anim = 0;
};