#include "../../Character.h"

class CharacterZero : public Character
{
public:
	CharacterZero(FightManager* mngr, b2Vec2 pos, char input,int p);
	virtual ~CharacterZero();
	
	

	virtual void draw() override;

	virtual void BasicNeutral(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;

	virtual void drawHUD( int numOfPlayer)override;
protected:
	void BuildBoxes() override {};

};