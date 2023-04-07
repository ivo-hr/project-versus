#pragma once
#include "Entity.h"

class Projectile abstract : public Entity
{
protected:
	int damage;
	int duration;
	int outFor;
	int lag = 6;
	b2Vec2 vecDir;
	HitData data;
	float ang;

	int reflected;

public:
	Projectile(FightManager* manager, b2Vec2 pos, b2Vec2 dir, float width, float height, int speed);
	~Projectile();

	void update() override;
	void draw() override;
	void draw(const SDL_Rect& camera);
	void CheckHits() override;
	SDL_Rect* GetHurtbox() { return &hurtbox;};
	bool changeDir();

};