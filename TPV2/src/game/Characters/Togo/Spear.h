#pragma once
#include "../../Projectile.h"

class Spear : public Projectile
{

public:
	Spear(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, Togo* togo);
	virtual ~Spear();

	string GetName() override { return "Togo_Spear"; };

	virtual void update() override;
	virtual void CheckHits() override;
	virtual bool GetHit(attackData a, Entity* attacker) { return(false);};
	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};

	virtual void OnDeath() override;

	virtual void draw(SDL_Rect* camera) override;

	SDL_Rect getCurrentSpriteSrc() override { return { 393, 395, 80, 9 }; }

private:
	Togo* owner = nullptr;
	
	int sprite = 0;
	float anim = 0;

	int hitDelay;
};