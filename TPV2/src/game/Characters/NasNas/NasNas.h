#include "../../Character.h"

class DinoShield;

class NasNas : public Character
{
public:
	NasNas(FightManager* mngr, b2Vec2 pos, char input,int p);
	~NasNas();

	virtual void BasicNeutral(int frameNumber) override;
	virtual void BasicForward(int frameNumber) override;
	virtual void BasicUpward(int frameNumber) override;
	virtual void BasicDownward(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;
	virtual void SpecialForward(int frameNumber) override;
	virtual void SpecialUpward(int frameNumber) override;
	virtual void SpecialDownward(int frameNumber) override;

	virtual void update() override;
	virtual void drawHUD(int w, int h, int numOfPlayer, int screenadjust)override;
private:
	int maxMana = 600;
	int mana = maxMana;
	state estado = fire;
};