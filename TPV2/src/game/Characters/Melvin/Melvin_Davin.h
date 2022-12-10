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

	virtual void BasicNeutral(ushort frameNumber) override;
	virtual void BasicForward(ushort frameNumber) override;
	virtual void BasicUpward(ushort frameNumber) override;
	virtual void BasicDownward(ushort frameNumber) override;

	virtual void SpecialNeutral(ushort frameNumber) override;
	virtual void SpecialForward(ushort frameNumber) override;
	virtual void SpecialUpward(ushort frameNumber) override;
	virtual void SpecialDownward(ushort frameNumber) override;

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