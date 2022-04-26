#include "../../Character.h"

class DinoShield;

class NasNas : public Character
{
public:
	NasNas(FightManager* mngr, Vector2D* pos, char input);
	~NasNas();

	virtual void BasicNeutral(int frameNumber) override;
	virtual void BasicForward(int frameNumber) override;
	virtual void BasicUpward(int frameNumber) override;
	virtual void BasicDownward(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;
	virtual void SpecialForward(int frameNumber) override;
	virtual void SpecialUpward(int frameNumber) override;
	virtual void SpecialDownward(int frameNumber) override;

	virtual bool GetHit(attackData a, Entity* attacker);
	virtual void update() override;
private:
	int maxMana = 360;
	int mana = maxMana;
	enum state estado = fire;
};