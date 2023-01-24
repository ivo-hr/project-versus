#include "../../Character.h"

class Melvin_Kyp;
class Melvin_Davin;
class Melvin_Cientifico;

class Melvin : public Character
{
public:
	Melvin(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	~Melvin();

	string GetName() override { return "Melvin"; };

	void update() override;

	void BasicNeutral(ushort frameNumber) override;
	void BasicForward(ushort frameNumber) override;
	void BasicUpward(ushort frameNumber) override;
	void BasicDownward(ushort frameNumber) override;

	void SpecialNeutral(ushort frameNumber) override;
	void SpecialForward(ushort frameNumber) override;
	void SpecialUpward(ushort frameNumber) override;
	void SpecialDownward(ushort frameNumber) override;

	bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;

	void SetSpawn(b2Vec2 spawn, short dir) override;

	void ReadyToPosses(Entity* attacker, bool& controlHitLag, bool& controlCamShake);
	void Posses();
	void UnPosses();

	void OnFightBegin() override;

	static void TransformInto(Character*, Character*);

	void HandledDelete() { kyp = nullptr; davin = nullptr; cientifico = nullptr; input = nullptr; };

private:

	Melvin_Kyp* kyp = nullptr;
	Melvin_Davin* davin = nullptr;
	Melvin_Cientifico* cientifico = nullptr;

	bool readyToPosses = false;
	InputConfig* possesedInput = nullptr;
	Character* toPosses = nullptr;
	ushort possesedCharIndex = USHRT_MAX;
	ushort possesedLayer = 0;
	ushort possesionTime = 300;
	ushort possesTimer = 0;

protected:
	void BuildBoxes() override;
};