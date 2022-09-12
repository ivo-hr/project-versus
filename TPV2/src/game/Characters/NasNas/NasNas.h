#include "../../Character.h"

class DinoShield;

class NasNas : public Character
{
public:
	NasNas(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~NasNas();

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;

	virtual void SpecialUpHit(ushort frameNumber);

	virtual void update() override;
	virtual void drawHUD(ushort numOfPlayer) override;
	virtual void Respawn() override;
private:
	ushort maxMana = 600;
	float mana = maxMana;
	float manaRecoverRate = 1.5f;
	state estado = fire;
protected:
	void BuildBoxes() override;
};