#include "../../Character.h"

class Melvin;
class Melvin_Davin;
class Melvin_Cientifico;

class Melvin_Kyp : public Character
{
public:
	Melvin_Kyp(FightManager* mngr, b2Vec2 pos, char input, InputConfig* input_, ushort p);
	~Melvin_Kyp();

	string GetName() override { return "Melvin_Kyp"; };

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;
	
	void SetOtherChar(Melvin* m, Melvin_Davin* d, Melvin_Cientifico* c) { melvin = m; davin = d; cientifico = c; };

	void HandledDelete() { melvin = nullptr; davin = nullptr; cientifico = nullptr; input = nullptr;
	};

private:

	Melvin* melvin = nullptr;
	Melvin_Davin* davin = nullptr;
	Melvin_Cientifico* cientifico = nullptr;

protected:
	void BuildBoxes() override;
};