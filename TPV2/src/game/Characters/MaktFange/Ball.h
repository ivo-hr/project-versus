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
	MaktBall(FightManager* manager, b2Vec2 pos, const HitData& attack, b2Vec2 dir, b2Vec2 respawn, ushort pNumber, ushort layer);
	virtual ~MaktBall();

	string GetName() override { return "Makt_Ball"; };

	virtual void update() override;
	virtual void CheckHits() override;
	virtual SDL_Rect* GetHurtbox() { return &hurtbox; };

	void SetOriginalLayer();

	virtual void OnDeath() override;

	bool PickUp();

	virtual void draw(SDL_Rect* camera) override;

	SDL_Rect getCurrentSpriteSrc() override { return { 1260, 1543, 17, 17 }; }

private:

	void Respawn();

	ushort originalLayer;

	Texture* arrowsTex = nullptr;
	SDL_Rect arrowSrc = SDL_Rect();

	bool physic = false;

	int sprite = 0;
	float anim = 0;
};