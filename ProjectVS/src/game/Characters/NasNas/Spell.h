#pragma once

#include "../../Projectile.h"


class Spell : public Projectile
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
	Spell(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, state state);
	~Spell();

	std::string GetName() override { return "Nas_Spell"; };

	SDL_Rect* GetHurtbox() { return &hurtbox;};

	void draw(const SDL_Rect& camera) override;

	SDL_Rect getCurrentSpriteSrc() override { return { 0, 1264, 35, 16 }; }

	int sprite = 0;
	float anim = 0;
};