#pragma once
#include "../../Character.h"

class YunoBubble;

class Yuno : public Character
{
public:
	Yuno(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~Yuno();

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

private:

	YunoBubble* bubble = nullptr;

	Texture* blinkContainer = nullptr;
	Texture* blinkfondo = nullptr;

	bool releasedSpec = false;

	bool boosted = false;

	bool casco = false;

protected:
	void BuildBoxes() override;

};