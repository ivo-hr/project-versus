#pragma once
#include "../../Character.h"

class YunoBubble;
enum Bubble { NEUTRAL, FORWARD, UP };

class Yuno : public Character
{
public:
	Yuno(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~Yuno();

	

	string GetName() override { return "Yuno"; };

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;

	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;

	virtual void BubblePopped();

	virtual void update() override;

	virtual void OnDeath() override;

	void setExplotado(bool dime) { explotado = dime; };
	bool getExplotado() { return explotado; };

private:

	YunoBubble* bubble = nullptr;

	bool releasedSpec = false;

	bool explotado = false;

	int lastBubble = 0;

	bool boosted = false;

	bool casco = false;

protected:
	void BuildBoxes() override;

};