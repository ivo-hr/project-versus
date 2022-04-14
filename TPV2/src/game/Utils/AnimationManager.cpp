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

			if (currentAnim.totalSprites - currentAnim.keySprite == 0)
			{
				framespSprite = currentAnim.totalFrames - currentAnim.hitboxFrame;
			}
			else
			{
				framespSprite = (int)((currentAnim.totalFrames - currentAnim.hitboxFrame) /
					(currentAnim.totalSprites - currentAnim.keySprite));
			}
		}

		//Si llega al final..
		if (currIndex == currentAnim.totalSprites + currentAnim.iniSprite)
		{
			if (!currentAnim.loop)
			{
				//Si no loopea inicia la animacion de idle
				StartAnimation("idle");
			}
			else
				//Si loopea reinicia la animacion
				currIndex = currentAnim.iniSprite;
		}

		//Reinicia el contador
		cont = framespSprite;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------

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

	//ent->GetManager()->GetScreenRatio() a 1920, 1080 esto es 3.75

	xOffset = (data.leftOffset * ent->GetWidth()) * ent->GetManager()->GetScreenRatio() / 4.7f;		//En zero (offset = 4, width = 3, tiene que dar APROX 10
	//xOffset = 10.f;
	yOffset = data.upOffset * ent->GetHeight() * ent->GetManager()->GetScreenRatio() / 4.7f;			//En zero (offset = 48, height = 3, tiene que dar APROX 115
	//yOffset = 115.f;


	//Este rect representa donde se va a renderizar la textura una vez recortada
	dest = *entity->GetHurtbox();

	dest.w += data.sizeXOffset * ent->GetWidth() * ent->GetManager()->GetScreenRatio() / 4.7f;		//En zero (offset = 28, width = 3, tiene que dar APROX 68
	//dest.w += 68;
	dest.h += data.sizeYOffset * ent->GetHeight() * ent->GetManager()->GetScreenRatio() / 4.7f;		//En zero (offset = 48, height = 3, tiene que dar APROX 115
	//dest.h += 115.f;

	//Inicializamos la animacion primera (en zero es idle)
	currentAnimIndex = "idle";
	currentAnim = data.animations["idle"];

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
	}
	else {
		dest.x = aux.x + xOffset - info.sizeXOffset * ent->GetWidth() * ent->GetManager()->GetScreenRatio() / 4.7f;
		dest.y = aux.y - yOffset;

		recorteSheet = { w * (int)SpriteIndex[currIndex].x, h * (int)SpriteIndex[currIndex].y, w, h };
	}

}

void AnimationManager::render()
{
	if (ent->GetDir() >= 0) {
		texture->render(recorteSheet, dest);
	}
	else {
		texture->render(recorteSheet, dest, 0., nullptr, SDL_FLIP_HORIZONTAL);
	}

	////If debug...
	//SDL_SetRenderDrawColor(ent->GetManager()->GetSDLU()->renderer(), 128, 128, 255, 255);
	//SDL_RenderDrawRect(ent->GetManager()->GetSDLU()->renderer(), &dest);

}

void AnimationManager::render(SDL_Rect* camera)
{
	SDL_Rect aux = dest;

	aux.x -= camera->x;
	aux.x *= (ent->GetManager()->GetActualWidth() / (float)camera->w);

	aux.y -= camera->y;
	aux.y *= (ent->GetManager()->GetActualHeight() / (float)camera->h);

	aux.w *= (ent->GetManager()->GetActualWidth() / (float)camera->w);
	aux.h *= (ent->GetManager()->GetActualHeight() / (float)camera->h);

	if (ent->GetDir() >= 0) {
		texture->render(recorteSheet, aux);
	}
	else {
		texture->render(recorteSheet, aux, 0., nullptr, SDL_FLIP_HORIZONTAL);
	}
}

void AnimationManager::StartAnimation(std::string index)
{
	currentAnimIndex = index;
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



