#pragma once
#include "../../Character.h"

class DinoShield;

class Togo : public Character
{
public:
	Togo(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~Togo();

	string GetName() override { return "Togo"; };

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;

	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;
	virtual void update() override;
	virtual void SetSpear(bool spear);
	void SpecialLHit(ushort frameNumber);
	virtual void draw(SDL_Rect* camera)override;
private:
	bool lanza = true;
	float spearTime;
	ushort attackBase;
	float attackMul;
	DinoShield* dShield = nullptr;
	SDL_Rect bite;
protected:
	void BuildBoxes() override;
};