#pragma once

#include "../../Projectile.h"


class MaktBall : public Projectile
{
	//protected:
	//
	//	int speed;
	//	int damage;
	//	int range = 20;
	//	Vector2D* position;
	//	Vector2D* iniPos;
	//	b2Vec2 vecDir;
	//	attackData data;
	//	float ang;

public:
	MaktBall(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, b2Vec2 respawn);
	virtual ~MaktBall();

	virtual void update() override;
	virtual void CheckHits() override;
	virtual SDL_Rect* GetHurtbox() { return &hurtbox; };

	virtual void OnDeath() override;

	bool PickUp();

	virtual void draw(SDL_Rect* camera) override;

private:

	void Respawn();

	bool physic;

	int sprite = 0;
	float anim = 0;
};