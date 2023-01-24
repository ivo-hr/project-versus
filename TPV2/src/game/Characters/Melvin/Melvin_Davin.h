#include "../../Character.h"

class Melvin_Kyp;
class Melvin;
class Melvin_Cientifico;

class Melvin_Davin : public Character
{

	friend class Melvin;

public:
	Melvin_Davin(FightManager* mngr, b2Vec2 pos, char input, InputConfig* input_, ushort p);
	~Melvin_Davin();

	string GetName() override { return "Melvin_Davin"; };

	void BasicNeutral(ushort frameNumber) override;
	void BasicForward(ushort frameNumber) override;
	void BasicUpward(ushort frameNumber) override;
	void BasicDownward(ushort frameNumber) override;

	void SpecialNeutral(ushort frameNumber) override;
	void SpecialForward(ushort frameNumber) override;
	void SpecialUpward(ushort frameNumber) override;
	void SpecialDownward(ushort frameNumber) override;

	void SetOtherChar(Melvin* m, Melvin_Kyp* k, Melvin_Cientifico* c) { melvin = m; kyp = k; cientifico = c; };

	void HandledDelete() { kyp = nullptr; melvin = nullptr; cientifico = nullptr; input = nullptr;
	};

private:

	Melvin* melvin = nullptr;
	Melvin_Kyp* kyp = nullptr;
	Melvin_Cientifico* cientifico = nullptr;

protected:
	void BuildBoxes() override;
};