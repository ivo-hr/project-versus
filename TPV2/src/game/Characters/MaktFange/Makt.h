#include "../../Character.h"

class MaktBall;

class Makt : public Character
{
public:
	Makt(FightManager* mngr, b2Vec2 pos, char input,int p);
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

	virtual void update() override;

	virtual bool GetHit(attackData a, Entity* attacker) override;
	virtual void StartJump(int frameNumber) override;
	virtual void StartShield(int frameNumber) override;
	virtual void EndShield(int frameNumber) override;
	virtual void Dash(int frameNumber) override;
	virtual void Taunt(int frameNumber) override;
	virtual void drawHUD( int numOfPlayer)override;
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
	void ThrowBall(attackData force, int timeHeld);

	MaktBall* ball;
};