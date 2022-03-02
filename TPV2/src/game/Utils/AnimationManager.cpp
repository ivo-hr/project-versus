#include "AnimationManager.h"




AnimationManager::AnimationManager(Texture* textura, Entity* entity, int x, int y):texture(textura)
{
	ent = entity;

	spriteNx = x + 1;
	spriteNy = y + 1;
	w = textura->width()/ spriteNx;
	h = textura->height()/ spriteNy;
	cont = 10;

	int a = 0;
	for (int i =0; i< spriteNy+1 ; i++)
	{
		for (int j = 0; j < spriteNx + 1; j++)
		{
			SpriteIndex.push_back(b2Vec2(j, i));
			a++;
		}
	}
	//w y h representan el tamaño de cada sprite
	//recorteSheet 
	recorteSheet = { w * spriteNx, h * spriteNy, w, h };

	//Este rect representa donde se va a renderizar la textura una vez recortada
	dest = *entity->GetHurtbox();

	dest.x -= 12;
	dest.y -= 120;
	dest.w += 72;
	dest.h += 120;

}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::idle()
{


}

void AnimationManager::update()
{
	cont--;
	if (cont == 0) {
		cont = 10;
		if (currIndex == 0)
			currIndex = 1;
		else
			currIndex = 0;
	}

	//Este rect representa donde se va a renderizar la textura una vez recortada
	dest = *ent->GetHurtbox();

	dest.x -= 12;
	dest.y -= 120;
	dest.w += 72;
	dest.h += 120;

	recorteSheet = { w * (int)SpriteIndex[currIndex].x, h * (int)SpriteIndex[currIndex].y, w, h };

	texture->render(recorteSheet, dest);

}

