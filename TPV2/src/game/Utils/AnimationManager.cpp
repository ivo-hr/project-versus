#include "AnimationManager.h"




void AnimationManager::UpdateIndex()
{
	cont--;
	if (cont == 0) {
		currIndex++;
		if (currIndex == currentAnim.keySprite + currentAnim.iniSprite)
		{
			framespSprite = (int)((currentAnim.totalFrames - currentAnim.hitboxFrame) / 
				(currentAnim.totalSprites - currentAnim.keySprite));
		}
		if (currIndex == currentAnim.totalSprites + currentAnim.iniSprite)
		{
			if (!currentAnim.loop)
			{
				StartAnimation(0);
			}
			else
				currIndex = currentAnim.iniSprite;
		}
		cont = framespSprite;
	}
}

AnimationManager::AnimationManager(Entity* entity, Texture* textura, spriteSheetData data) : texture(textura), info(data)
{
	ent = entity;

	w = textura->width() / data.spritesInX;
	h = textura->height() / data.spritesInY;

	for (int i =0; i < data.spritesInY; i++)
	{
		for (int j = 0; j < data.spritesInX; j++)
		{
			SpriteIndex.push_back(b2Vec2(j, i));
		}
	}
	//w y h representan el tamaño de cada sprite
	//recorteSheet 
	recorteSheet = { w * data.spritesInX, h * data.spritesInY, w, h };

	xOffset = data.leftOffset * ent->GetWidth();
	yOffset = data.upOffset * ent->GetHeight();

	//Este rect representa donde se va a renderizar la textura una vez recortada
	dest = *entity->GetHurtbox();

	dest.w += data.sizeXOffset * ent->GetWidth();
	dest.h += data.sizeYOffset * ent->GetHeight();


	currentAnim = data.animations[0];
	currIndex = currentAnim.iniSprite;
	framespSprite = (int)(currentAnim.totalFrames / currentAnim.totalSprites);

	cont = framespSprite;
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::idle()
{


}

void AnimationManager::update()
{

	UpdateIndex();

	//Este rect representa donde se va a renderizar la textura una vez recortada
	SDL_Rect aux = *ent->GetHurtbox();

	dest.x = aux.x - xOffset;
	dest.y = aux.y - yOffset;

	recorteSheet = { w * (int)SpriteIndex[currIndex].x, h * (int)SpriteIndex[currIndex].y, w, h };

	texture->render(recorteSheet, dest);

}

void AnimationManager::StartAnimation(int index)
{
	currentAnim = info.animations[index];
	currIndex = currentAnim.iniSprite;
	if (currentAnim.keySprite == -1)
		framespSprite = (int)(currentAnim.totalFrames / currentAnim.totalSprites);
	else
		framespSprite = (int)(currentAnim.hitboxFrame / currentAnim.keySprite);

	cont = framespSprite;
}



