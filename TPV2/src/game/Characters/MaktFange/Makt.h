#include "../../Character.h"

class MaktBall;

class Makt : public Character
{
public:
	Makt(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~Makt();

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

	void RecoveredBall();
	void ThrowBall(HitData force, ushort timeHeld);

	MaktBall* ball;
protected:
	void BuildBoxes() override;
};