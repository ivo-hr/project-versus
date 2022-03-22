#include "../../Character.h"

class GatoEspia : public Character
{
public:
	GatoEspia(FightManager* mngr, Vector2D* pos, char input);
	~GatoEspia();
	
	

	virtual void draw() override;

	virtual void BasicNeutral(int frameNumber) override;
	virtual void BasicForward(int frameNumber) override;
	virtual void BasicUpward(int frameNumber) override;
	virtual void BasicDownward(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;
	virtual void SpecialForward(int frameNumber) override;
	virtual void SpecialUpward(int frameNumber) override;
	virtual void SpecialDownward(int frameNumber) override;
	
	void TpAtack(int frameNumber);

};