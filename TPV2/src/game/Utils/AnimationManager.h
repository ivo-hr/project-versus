#pragma once

#include "../Entity.h"

class AnimationManager
{
protected:

	int currIndex = 0;

	int xOffset, yOffset;
	int framespSprite;
	int w, h, cont;
	std::vector<b2Vec2> SpriteIndex;
	std::vector<std::vector<int>> AnimationsInfo;
	Texture* texture;
	SDL_Rect recorteSheet;
	SDL_Rect dest;

	Entity* ent;
	spriteSheetData info;
	animationData currentAnim;

	void UpdateIndex();

public:

	AnimationManager(Entity* entity, Texture* textura, spriteSheetData data);
	~AnimationManager();

	void idle();
	void update();

	void StartAnimation(int index);
};