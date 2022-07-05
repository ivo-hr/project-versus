#include "../../Character.h"

class MaktBall;

class Makt : public Character
{
public:
	Makt(FightManager* mngr, b2Vec2 pos, char input,int p);
	virtual ~Makt();

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

	int baseWeight;
	int ballWeight;

	int baseSpeed;
	int ballSpeed;

	int baseJump;
	int ballJump;
	bool release = false;
	int frameRelease;
	void RecoveredBall();
	void ThrowBall(HitData force, int timeHeld);

	MaktBall* ball;
protected:
	void BuildBoxes() override;
};