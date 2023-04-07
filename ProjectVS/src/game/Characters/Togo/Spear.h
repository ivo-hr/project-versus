#pragma once
#include "../../Projectile.h"

class Spear : public Projectile
{

public:
	Spear(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, Togo* togo);
	~Spear();

	string GetName() override { return "Togo_Spear"; };

	void update() override;
	void CheckHits() override;
	bool GetHit(attackData a, Entity* attacker) { return(false);};
	SDL_Rect* GetHurtbox() { return &hurtbox;};

	void OnDeath() override;

	void draw(const SDL_Rect& camera) override;

	SDL_Rect getCurrentSpriteSrc() override { return { 393, 395, 80, 9 }; }

private:
	Togo* owner = nullptr;
	
	int sprite = 0;
	float anim = 0;

	int hitDelay;
};