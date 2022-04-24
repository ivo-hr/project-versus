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
	MaktBall(FightManager* manager, Vector2D* pos, attackData attack, b2Vec2 dir, Vector2D* respawn);
	~MaktBall();

	virtual void update() override;
	virtual void CheckHits() override;
	virtual bool GetHit(attackData a, Entity* attacker) { return(false); };
	virtual SDL_Rect* GetHurtbox() { return &hurtbox; };

	virtual void OnDeath() override;

	bool PickUp();

private:

	void Respawn();

	bool physic;
};