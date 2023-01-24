#pragma once
#include "../../Entity.h"

class DinoShield : public Entity
{
protected:
	attackData data;
public:
	DinoShield(FightManager* manager, b2Vec2 pos);
	~DinoShield();

	string GetName() override { return "Togo_Shield"; };

	void update() override;
	void draw() override;
	void draw(SDL_Rect* camera) override;
	void CheckHits() override {};
	bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;
	SDL_Rect* GetHurtbox() { return &hurtbox; };
	void setToDelete() { toDelete = true; }
private:
	int spriteX = 896;
	int spriteY = 82;
	float anim = 0;
};