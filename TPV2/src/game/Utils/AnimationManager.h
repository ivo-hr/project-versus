#pragma once

#include "../Entity.h"

class AnimationManager
{
protected:

	int currIndex = 0;

	int xOffset, yOffset;
	int framespSprite;
	int w, h, cont;
	bool lookingRight;

	std::string currentAnimIndex;

	std::vector<b2Vec2> SpriteIndex;
	Texture* texture;
	SDL_Rect recorteSheet;
	SDL_Rect dest;

	Entity* ent;
	spriteSheetData info;
	animationData* currentAnim = nullptr;

	int currentState;

	void UpdateIndex();

public:

	AnimationManager(Entity* entity, Texture* textura, spriteSheetData data);
	virtual ~AnimationManager();

	void update();
	void render();
	void render(SDL_Rect* camera);
	void render(SDL_Rect* camera, const Vector2D& offset);

	void StartAnimation(std::string index);
	std::string CurrentAnimation() { return currentAnimIndex; };

	void ChangeSheet(Texture* textura) { texture = textura; };

	int GetAnimationDuration();
};