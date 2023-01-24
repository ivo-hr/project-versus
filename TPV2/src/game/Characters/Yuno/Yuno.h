#pragma once
#include "../../Character.h"

class YunoBubble;
enum class Bubble { NEUTRAL, FORWARD, UP };

class Yuno : public Character
{
public:
	Yuno(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	~Yuno();

	string GetName() override { return "Yuno"; };

	void BasicNeutral(ushort frameNumber) override;
	void BasicForward(ushort frameNumber) override;
	void BasicUpward(ushort frameNumber) override;
	void BasicDownward(ushort frameNumber) override;

	void SpecialNeutral(ushort frameNumber) override;
	void SpecialForward(ushort frameNumber) override;
	void SpecialUpward(ushort frameNumber) override;
	void SpecialDownward(ushort frameNumber) override;

	bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;

	void BubblePopped();

	void update() override;

	void OnDeath() override;

	bool IsOutOfBounds() override;

	void setExplotado(bool dime) { explotado = dime; };

	bool getExplotado() { return explotado; };

private:

	YunoBubble* bubble = nullptr;

	bool releasedSpec = false;

	bool explotado = false;

	int lastBubble = 0;

	bool bubbled = false;

	bool boosted = false;

	bool casco = false;

protected:
	void BuildBoxes() override;

};