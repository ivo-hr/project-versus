#include "../../Character.h"

class CharacterZero : public Character
{
public:
	CharacterZero(FightManager* mngr, b2Vec2 pos, char input,int p);
	virtual ~CharacterZero();
	
	

	virtual void draw() override;

	virtual void BasicNeutral(int frameNumber) override;
	virtual void BasicForward(int frameNumber) override;
	virtual void BasicUpward(int frameNumber) override;
	virtual void BasicDownward(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;
	virtual void SpecialForward(int frameNumber) override;
	virtual void SpecialUpward(int frameNumber) override;
	virtual void SpecialDownward(int frameNumber) override;

	virtual void drawHUD( int numOfPlayer)override;
protected:
	void BuildBoxes() override {};

};