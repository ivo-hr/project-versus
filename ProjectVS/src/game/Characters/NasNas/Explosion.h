#pragma once

#include "../../Entity.h"


class Explosion : public Entity
{
protected:
	HitData data;
public:
	Explosion(FightManager* manager, b2Vec2 pos, int power, int type, bool lookRight);
	~Explosion();

	string GetName() override { return "Nas_Explosion"; };

	void update() override;
	void draw() override;
	void draw(SDL_Rect* camera) override;
	void CheckHits() override;
	SDL_Rect* GetHurtbox() { return &hurtbox; };
private:
	bool type;

	int anim = 0;
	int spDur = 0;

	int duration = 20;
	int time = 0;

	int spriteX = 0;
	int spriteY = 0;
};