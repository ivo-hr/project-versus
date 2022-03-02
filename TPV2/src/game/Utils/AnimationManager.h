#pragma once

#include "../Entity.h"

class AnimationManager
{
protected:
	int spriteNx;
	int spriteNy;

	int currIndex = 0;

	int w, h, cont;
	std::vector<b2Vec2> SpriteIndex;
	std::vector<std::vector<int>> AnimationsInfo;
	Texture* texture;
	SDL_Rect recorteSheet;
	SDL_Rect dest;

	Entity* ent;
public:

	AnimationManager(Texture* textura, Entity* entity, int x, int y);
	~AnimationManager();
	void idle();
	void update();

};