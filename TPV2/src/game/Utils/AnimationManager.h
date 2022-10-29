#pragma once

#include "../Entity.h"

class AnimationManager
{
protected:

	ushort currIndex = 0;

	short xOffset, yOffset;
	ushort framespSprite;
	ushort w, h, cont;
	bool lookingRight;

	std::string currentAnimIndex;

	std::vector<Vector2D> SpriteIndex;
	Texture*& texture;
	SDL_Rect recorteSheet;
	SDL_Rect dest;

	Entity* ent;
	spriteSheetData info;
	animationData* currentAnim = nullptr;

	ushort currentState;

	void UpdateIndex();

public:

	AnimationManager(Entity* entity, Texture*& textura, spriteSheetData data);
	virtual ~AnimationManager();

	void update();
	void render();
	void render(SDL_Rect* camera);
	void render(SDL_Rect* camera, const Vector2D& offset);

	void StartAnimation(std::string index);
	std::string CurrentAnimation() { return currentAnimIndex; };

	int GetAnimationDuration();
	SDL_Rect& getCurrentSpriteSrc() { return recorteSheet; }
};