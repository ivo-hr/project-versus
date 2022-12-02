#include "../../Character.h"

class Melvin_Kyp;
class Melvin_Davin;
class Melvin;

class Melvin_Cientifico : public Character
{
public:
	Melvin_Cientifico(FightManager* mngr, b2Vec2 pos, char input, InputConfig* input_, ushort p);
	~Melvin_Cientifico();

	string GetName() override { return "Melvin_Cientifico"; };

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;

	void SetOtherChar(Melvin* m, Melvin_Davin* d, Melvin_Kyp* k) { melvin = m; davin = d; kyp = k; };

	void HandledDelete() { kyp = nullptr; davin = nullptr; melvin = nullptr; input = nullptr;
	};

private:

	Melvin* melvin = nullptr;
	Melvin_Kyp* kyp = nullptr;
	Melvin_Davin* davin = nullptr;

protected:
	void BuildBoxes() override;
};