#include "../../Character.h"

class CharacterZero : public Character
{
public:
	CharacterZero(FightManager* mngr, b2Vec2 pos, char input, ushort p);
	virtual ~CharacterZero();

	string GetName() override { return "Ciro"; };

	virtual void draw() override;

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;

protected:
	void BuildBoxes() override;

};