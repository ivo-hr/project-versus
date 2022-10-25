#include "../../Character.h"

class GatoEspia : public Character
{
public:
	GatoEspia(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~GatoEspia();

	string GetName() override { return "Blink"; };

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialNeutralU(ushort frameNumber);
	virtual void SpecialNeutralD(ushort frameNumber);
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;
	
	void TpAtack(ushort frameNumber);

	virtual void update() override;
	virtual void Respawn() override;
	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;
	void Counter(ushort frameNumber);

	virtual void drawHUD(ushort numOfPlayer)override;
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