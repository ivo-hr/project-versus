#pragma once
#include "../../Entity.h"
#include "Yuno.h"

//class Yuno;
class InputConfig;

class YunoBubble : public Entity
{
public:
	YunoBubble(FightManager* manager, b2Vec2 pos, Yuno* owner, int iniSpan, int moreSpan, Bubble bubbleType, InputConfig* input = nullptr);
	~YunoBubble();

	string GetName() override { return "Yuno_Bubble"; };

	void update() override;
	void draw() override { };
	void draw(const SDL_Rect& camera) override;
	void CheckHits() override;
	bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;
	SDL_Rect* GetHurtbox() { return &hurtbox; };
	void setToDelete() { toDelete = true; timeSinceBubble = 0; yuno->BubblePopped(); };
	void OnDeath() override;
	bool IsOutOfBounds() override { return !SDL_HasIntersection(&hurtbox, &bubbleDeathZone); }

	void GetInsideBubble(Entity* ent);
	void Pop();
	void Explode();

private:
	Yuno* yuno = nullptr;

	Texture* arrowsTex = nullptr;
	SDL_Rect arrowSrc = SDL_Rect();

	Entity* bubbledEntity = nullptr;
	SDL_Rect texSrc = SDL_Rect();
	SDL_Rect texDest = SDL_Rect();

	InputConfig* hndlr = nullptr;

	bool toExplode = false;

	float totalLifespan = 0;
	float lifespan = 0;
	int moarSpan = 0;
	int timeSinceBubble = 0;
	float invFrames = 0;

	SDL_Rect bubbleDeathZone = {};

	Bubble pompa;

};