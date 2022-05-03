#include "../../Character.h"

class GatoEspia : public Character
{
public:
	GatoEspia(FightManager* mngr, b2Vec2 pos, char input,int p);
	virtual ~GatoEspia();


	virtual void BasicNeutral(int frameNumber) override;
	virtual void BasicForward(int frameNumber) override;
	virtual void BasicUpward(int frameNumber) override;
	virtual void BasicDownward(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;
	virtual void SpecialNeutralU(int frameNumber);
	virtual void SpecialNeutralD(int frameNumber);
	virtual void SpecialForward(int frameNumber) override;
	virtual void SpecialUpward(int frameNumber) override;
	virtual void SpecialDownward(int frameNumber) override;
	
	void TpAtack(int frameNumber);

	virtual void update() override;
	virtual void Respawn() override;
	virtual bool GetHit(attackData a, Entity* attacker) override;
	void Counter(int frameNumber);

	virtual void drawHUD( int numOfPlayer)override;
private:

	Texture* blinkContainer = nullptr;
	Texture* blinkfondo = nullptr;

	float maxBlinks = 3.2;
	float blinks = maxBlinks;
	float blinkRecover = 0.008;
	bool counter = false;

};