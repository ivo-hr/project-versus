#include "../../Character.h"

class MaktBall;

class Makt : public Character
{
public:
	Makt(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	~Makt();

	string GetName() override { return "Makt"; };

	void BasicNeutral(ushort frameNumber) override;
	void BasicForward(ushort frameNumber) override;
	void BasicUpward(ushort frameNumber) override;
	void BasicDownward(ushort frameNumber) override;

	void SpecialNeutral(ushort frameNumber) override;
	void SpecialForward(ushort frameNumber) override;
	void SpecialUpward(ushort frameNumber) override;
	void SpecialDownward(ushort frameNumber) override;

	void ThrowRecover(ushort frameNumber);
	void BallPickUp(ushort frameNumber);

	void update() override;
	bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;

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