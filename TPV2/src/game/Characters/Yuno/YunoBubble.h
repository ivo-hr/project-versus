#pragma once
#include "../../Entity.h"
#include "Yuno.h"

//class Yuno;
class InputConfig;

class YunoBubble : public Entity
{

public:
	YunoBubble(FightManager* manager, b2Vec2 pos, Yuno* owner, InputConfig* input = nullptr);
	virtual ~YunoBubble();

	virtual void update() override;
	virtual void draw() override { };
	virtual void draw(SDL_Rect* camera) override;
	virtual void CheckHits() override;
	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;
	virtual SDL_Rect* GetHurtbox() { return &hurtbox; };
	virtual void setToDelete() { toDelete = true; yuno->BubblePopped(); };
	virtual void OnDeath() override;

	void GetInsideBubble(Entity* ent);
	void Pop();

private:

	Yuno* yuno = nullptr;

	Texture* arrowsTex = nullptr;
	SDL_Rect arrowSrc = SDL_Rect();

	Entity* bubbledEntity = nullptr;
	SDL_Rect texSrc = SDL_Rect();
	SDL_Rect texDest = SDL_Rect();

	InputConfig* hndlr = nullptr;

};