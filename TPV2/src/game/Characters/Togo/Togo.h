#include "../../Character.h"

class Togo : public Character
{
public:
	Togo(FightManager* mngr, Vector2D* pos, char input);
	~Togo();


	virtual void BasicNeutral(int frameNumber) override;
	virtual void BasicForward(int frameNumber) override;
	virtual void BasicUpward(int frameNumber) override;
	virtual void BasicDownward(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;
	virtual void SpecialForward(int frameNumber) override;
	virtual void SpecialUpward(int frameNumber) override;
	virtual void SpecialDownward(int frameNumber) override;

private:

};