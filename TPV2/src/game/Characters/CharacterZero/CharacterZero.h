#include "../../Character.h"

class CharacterZero : public Character
{
public:
	CharacterZero(FightManager* mngr, b2Vec2 pos, char input,int p);
	~CharacterZero();
	
	

	virtual void draw() override;

	virtual void BasicNeutral(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;

	virtual void drawHUD(int w, int h, int numOfPlayer, int screenadjust)override;

};