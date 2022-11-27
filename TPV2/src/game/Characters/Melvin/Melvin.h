#include "../../Character.h"

class Melvin : public Character
{
public:
	Melvin(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	~Melvin();

	string GetName() override { return "Melvin"; };

	virtual void update() override;

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;

	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;

	void Posses(Entity* attacker, bool& controlHitLag, bool& controlCamShake);
	void UnPosses();

private:

	bool readyToPosses = false;
	InputConfig* possesedInput = nullptr;
	Character* toPosses = nullptr;
	Character* possesedChar = nullptr;
	ushort possesedLayer = 0;
	ushort possesionTime = 180;
	ushort possesTimer = 0;

protected:
	void BuildBoxes() override;
};