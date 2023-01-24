#include "../../Character.h"

class NasNas : public Character
{
public:
	NasNas(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	~NasNas();

	string GetName() override { return "NasNas"; };

	void BasicNeutral(ushort frameNumber) override;
	void BasicForward(ushort frameNumber) override;
	void BasicUpward(ushort frameNumber) override;
	void BasicDownward(ushort frameNumber) override;

	void SpecialNeutral(ushort frameNumber) override;
	void SpecialForward(ushort frameNumber) override;
	void SpecialUpward(ushort frameNumber) override;
	void SpecialDownward(ushort frameNumber) override;

	void SpecialUpHit(ushort frameNumber);

	void update() override;
	void drawHUD(ushort numOfPlayer) override;
	void Respawn() override;
private:
	ushort maxMana = 600;
	float mana = maxMana;
	float manaRecoverRate = 1.5f;
	state estado = fire;
protected:
	void BuildBoxes() override;
};