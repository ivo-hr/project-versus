#include "../../Character.h"

class CharacterZero : public Character
{
public:
	CharacterZero(FightManager* mngr, Vector2D* pos, char input);
	~CharacterZero();
	
	

	virtual void draw() override;

	virtual void BasicNeutral(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;



};