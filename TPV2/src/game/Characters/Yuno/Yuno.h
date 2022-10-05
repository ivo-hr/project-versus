#include "../../Character.h"

class Yuno : public Character
{
public:
	Yuno(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~Yuno();


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

private:

	Texture* blinkContainer = nullptr;
	Texture* blinkfondo = nullptr;

	bool boosted = false;

protected:
	void BuildBoxes() override;

};