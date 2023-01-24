#include "../../Character.h"

class GatoEspia : public Character
{
public:
	GatoEspia(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	~GatoEspia();

	string GetName() override { return "Blink"; };

	void BasicNeutral(ushort frameNumber) override;
	void BasicForward(ushort frameNumber) override;
	void BasicUpward(ushort frameNumber) override;
	void BasicDownward(ushort frameNumber) override;

	void SpecialNeutral(ushort frameNumber) override;
	void SpecialNeutralU(ushort frameNumber);
	void SpecialNeutralD(ushort frameNumber);
	void SpecialForward(ushort frameNumber) override;
	void SpecialUpward(ushort frameNumber) override;
	void SpecialDownward(ushort frameNumber) override;
	
	void TpAtack(ushort frameNumber);

	void update() override;
	void Respawn() override;
	bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;
	void Counter(ushort frameNumber);

	void drawHUD(ushort numOfPlayer)override;
private:

	Texture* blinkContainer = nullptr;
	Texture* blinkfondo = nullptr;

	float maxBlinks = 3.2f;
	float blinks = maxBlinks;
	float blinkRecover = 0.008f;
	bool counter = false;

protected:
	void BuildBoxes() override;

};