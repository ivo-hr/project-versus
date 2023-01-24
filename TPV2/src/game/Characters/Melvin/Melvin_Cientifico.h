#include "../../Character.h"

class Melvin_Kyp;
class Melvin_Davin;
class Melvin;

class Melvin_Cientifico : public Character
{

	friend class Melvin;

public:
	Melvin_Cientifico(FightManager* mngr, b2Vec2 pos, char input, InputConfig* input_, ushort p);
	~Melvin_Cientifico();

	string GetName() override { return "Melvin_Cientifico"; };

	void BasicNeutral(ushort frameNumber) override;
	void BasicForward(ushort frameNumber) override;
	void BasicUpward(ushort frameNumber) override;
	void BasicDownward(ushort frameNumber) override;

	void SpecialNeutral(ushort frameNumber) override;
	void SpecialForward(ushort frameNumber) override;
	void SpecialUpward(ushort frameNumber) override;
	void SpecialDownward(ushort frameNumber) override;

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