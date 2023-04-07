#pragma once
#include "../../Character.h"

class DinoShield;

class Togo : public Character
{
public:
	Togo(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	~Togo();

	string GetName() override { return "Togo"; };

	void BasicNeutral(ushort frameNumber) override;
	void BasicForward(ushort frameNumber) override;
	void BasicUpward(ushort frameNumber) override;
	void BasicDownward(ushort frameNumber) override;

	void SpecialNeutral(ushort frameNumber) override;
	void SpecialForward(ushort frameNumber) override;
	void SpecialUpward(ushort frameNumber) override;
	void SpecialDownward(ushort frameNumber) override;

	bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;
	void update() override;
	void SetSpear(bool spear);
	void SpecialLHit(ushort frameNumber);
	void draw(const SDL_Rect& camera) override;

	void OnParry(Entity* attacker, bool& controlHitLag, HitData& a, bool& controlCamShake, bool& controlShake) override;

private:
	bool lanza = true;
	float spearTime;
	ushort attackBase;
	float attackMul;
	DinoShield* dShield = nullptr;
	SDL_Rect bite;
protected:

	void BuildParticlePool() override;
	void BuildBoxes() override;

	void ResetChar() override;
};