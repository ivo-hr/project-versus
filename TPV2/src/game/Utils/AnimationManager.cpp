#include "AnimationManager.h"




void AnimationManager::UpdateIndex()
{
	//Restamos 1 al contador
	cont--;
	//Cuando llegue a 0...
	if (cont == 0) {
		//Aumenta el índice para renderizar el siguiente sprite
		currIndex++;

		//Si ha llegado al keySprite...
		if (currIndex == currentAnim.keySprite + currentAnim.iniSprite)
		{
			//Vuelve a calcular los frames que dura cada sprite para que el final coincida con el final del movimiento
			framespSprite = (int)((currentAnim.totalFrames - currentAnim.hitboxFrame) / 
				(currentAnim.totalSprites - currentAnim.keySprite));
		}

		//Si llega al final..
		if (currIndex == currentAnim.totalSprites + currentAnim.iniSprite)
		{
			if (!currentAnim.loop)
			{
				//Si no loopea inicia la animacion de idle
				StartAnimation(0);
			}
			else
				//Si loopea reinicia la animacion
				currIndex = currentAnim.iniSprite;
		}

		//Reinicia el contador
		cont = framespSprite;
	}
}

AnimationManager::AnimationManager(Entity* entity, Texture* textura, spriteSheetData data) : texture(textura), info(data)
{
	ent = entity;

	w = textura->width() / data.spritesInX;			//Sacamos la anchura de cada sprite
	h = textura->height() / data.spritesInY;		//bruh

	for (int i =0; i < data.spritesInY; i++)
	{
		for (int j = 0; j < data.spritesInX; j++)
		{
			//Creamos el vector del cual sacaremos todos los sprites por separado
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

	//Inicializamos la animacion primera (en zero es idle)
	currentAnim = data.animations[0];

	//El index es el primer sprite
	currIndex = currentAnim.iniSprite;

	//Calculamos cuantos frames tienen que mantenerse cada sprite
	framespSprite = (int)(currentAnim.totalFrames / currentAnim.totalSprites);

	//Inicializamos el contador
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

	//Actualizamos el Indice de la spritesheet si es necesario
	UpdateIndex();

	//Este rect representa donde se va a renderizar la textura una vez recortada
	SDL_Rect aux = *ent->GetHurtbox();

	//Depende de a donde esté mirando, el cuadro donde se renderiza se calcula de una manera u otra
	if (ent->GetDir() > 0) {
		dest.x = aux.x - xOffset;
		dest.y = aux.y - yOffset;

		recorteSheet = { w * (int)SpriteIndex[currIndex].x, h * (int)SpriteIndex[currIndex].y, w, h };

		texture->render(recorteSheet, dest);
	}
	else {
		dest.x = aux.x + xOffset - info.sizeXOffset * ent->GetWidth();
		dest.y = aux.y - yOffset;

		recorteSheet = { w * (int)SpriteIndex[currIndex].x, h * (int)SpriteIndex[currIndex].y, w, h };
		texture->render(recorteSheet, dest, 0., nullptr, SDL_FLIP_HORIZONTAL);
	}

}

void AnimationManager::StartAnimation(int index)
{
	//La animacion actual cambia a ser la nueva especificada
	currentAnim = info.animations[index];

	//El indice será el primero que esté en los datos
	currIndex = currentAnim.iniSprite;

	//Calcula cuanto debe durar cada sprite para que...
	if (currentAnim.keySprite == -1 || currentAnim.hitboxFrame == -1)
		//El key Sprite coincida con la hitbox
		framespSprite = (int)(currentAnim.totalFrames / currentAnim.totalSprites);
	else
		//Acabe a la vez que el movimiento
		framespSprite = (int)(currentAnim.hitboxFrame / currentAnim.keySprite);

	//Inicializamos el contador
	cont = framespSprite;
}



