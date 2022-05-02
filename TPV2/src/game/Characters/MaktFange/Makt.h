#include "../../Character.h"

class MaktBall;

class Makt : public Character
{
public:
	Makt(FightManager* mngr, b2Vec2 pos, char input);
	~Makt();

	virtual void BasicNeutral(int frameNumber) override;
	virtual void BasicForward(int frameNumber) override;
	virtual void BasicUpward(int frameNumber) override;
	virtual void BasicDownward(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;
	virtual void SpecialForward(int frameNumber) override;
	virtual void SpecialUpward(int frameNumber) override;
	virtual void SpecialDownward(int frameNumber) override;

	virtual void ThrowRecover(int frameNumber);
	virtual void BallPickUp(int frameNumber);

private:
	MaktBall* ball;

	int baseWeight;
	int ballWeight;

	int baseSpeed;
	int ballSpeed;

	int baseJump;
	int ballJump;

	void RecoveredBall();
	void ThrowBall(attackData force, int timeHeld);
};