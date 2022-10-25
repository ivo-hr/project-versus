#include "../../Character.h"

class MaktBall;

class Makt : public Character
{
public:
	Makt(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~Makt();

	string GetName() override { return "Makt"; };

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;

	virtual void ThrowRecover(ushort frameNumber);
	virtual void BallPickUp(ushort frameNumber);

	virtual void update() override;
	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;

private:

	ushort baseWeight;
	ushort ballWeight;

	ushort baseSpeed;
	ushort ballSpeed;

	ushort baseJump;
	ushort ballJump;
	bool release = false;
	ushort frameRelease;
	ushort timeHeld = 0;
	ushort ballRecover = 0;
	ushort maxRecover = 30;
	bool superArmor = false;

	void RecoveredBall();
	void ThrowBall(HitData force, ushort timeHeld);

	MaktBall* ball;
protected:
	void BuildBoxes() override;
};