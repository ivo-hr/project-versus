#include "AnimationManager.h"
#include "../../utils/CheckML.h"


void AnimationManager::UpdateIndex()
{
	//Restamos 1 al contador
	cont--;
	//Cuando llegue a 0...
	if (cont == 0) {
		//Aumenta el índice para renderizar el siguiente sprite
		currIndex++;

		if (currentAnim->keySprite.size() == 0)
		{
			if (currIndex == currentAnim->totalSprites + currentAnim->iniSprite)
			{
				if (!currentAnim->loop)
				{
					//Si no loopea inicia la animacion de idle
					StartAnimation("idle");
				}
				else
				{
					//Si loopea reinicia la animacion
					StartAnimation(currentAnimIndex);
				}
			}
		}
		else
		{
			if (currentState == currentAnim->keySprite.size())
			{
				if (currIndex == currentAnim->totalSprites + currentAnim->iniSprite)
				{
					if (!currentAnim->loop)
					{
						//Si no loopea inicia la animacion de idle
						StartAnimation("idle");
					}
					else
					{
						//Si loopea reinicia la animacion
						StartAnimation(currentAnimIndex);
					}
				}
			}
			else
			{
				if (currIndex >= currentAnim->keySprite[currentState] + currentAnim->iniSprite)
				{
					currentState++;
					if (currentState == currentAnim->keySprite.size())
					{
						if (currentAnim->totalSprites - currentAnim->keySprite[currentState - 1] == 0)
						{
							framespSprite = currentAnim->totalFrames - currentAnim->keyFrame[currentState - 1];
						}
						else
						{
							framespSprite = (int)((currentAnim->totalFrames - currentAnim->keyFrame[currentState - 1]) /
							(currentAnim->totalSprites - currentAnim->keySprite[currentState - 1]));
						}
					}
					else
					{
						if (currentAnim->keySprite[currentState] - currentAnim->keySprite[currentState - 1] == 0)
						{
							framespSprite = currentAnim->keyFrame[currentState] - currentAnim->keyFrame[currentState - 1];
						}
						else
						{
							framespSprite = (int)((currentAnim->keyFrame[currentState] - currentAnim->keyFrame[currentState - 1]) /
							(currentAnim->keySprite[currentState] - currentAnim->keySprite[currentState - 1]));
						}
					}
					if (currIndex > currentAnim->keySprite[currentState - 1] + currentAnim->iniSprite)
					{
						currIndex = currentAnim->keySprite[currentState - 1] + currentAnim->iniSprite;
					}
				}
			}
		}

		//Reinicia el contador
		cont = framespSprite;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------

AnimationManager::AnimationManager(Entity* entity, Texture*& textura, spriteSheetData data) : texture(textura), info(data)
{
	ent = entity;

	w = textura->width() / data.spritesInX;			//Sacamos la anchura de cada sprite
	h = textura->height() / data.spritesInY;		//bruh

	data.size = 1.15f * w;

	for (int i = 0; i < data.spritesInY; i++)
	{
		for (int j = 0; j < data.spritesInX; j++)
		{
			//Creamos el vector del cual sacaremos todos los sprites por separado
			SpriteIndex.push_back(Vector2D((float)j, (float)i));
		}
	}
	//w y h representan el tamaño de cada sprite
	//recorteSheet 
	recorteSheet = { w * data.spritesInX, h * data.spritesInY, w, h };

	//ent->GetManager()->GetScreenRatio() a 1920, 1080 esto es 3.75

	xOffset = (short)(((float)data.leftOffset * ent->GetWidth()) * ent->GetManager()->GetScreenRatio() / 4.7f);		//En zero (offset = 4, width = 3, tiene que dar APROX 10
	//xOffset = 10.f;
	yOffset = (short)(((float)data.upOffset * ent->GetHeight()) * ent->GetManager()->GetScreenRatio() / 4.7f);		//En zero (offset = 48, height = 3, tiene que dar APROX 115
	//yOffset = 115.f;


	//Este rect representa donde se va a renderizar la textura una vez recortada
	dest = { 0, 0, data.size, (int)(data.size * ((float)h / (float)w)) };


	//Inicializamos la animacion primera (en zero es idle)
	StartAnimation("idle");
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

		recorteSheet = { w * (int)SpriteIndex[currIndex].getX(), h * (int)SpriteIndex[currIndex].getY(), w, h };

		lookingRight = true;
	}
	else {
		dest.x = aux.x + aux.w + xOffset - dest.w;
		dest.y = aux.y - yOffset;

		recorteSheet = { w * (int)SpriteIndex[currIndex].getX(), h * (int)SpriteIndex[currIndex].getY(), w, h };

		lookingRight = false;
	}

}

void AnimationManager::render()
{
	if (lookingRight) {
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

	float wDiff = (float)ent->GetManager()->GetActualWidth() / (float)camera->w;
	float hDiff = (float)ent->GetManager()->GetActualHeight() / (float)camera->h;

	aux.x -= camera->x;
	aux.x = (int)((float)aux.x * wDiff);

	aux.y -= camera->y;
	aux.y = (int)((float)aux.y * hDiff);

	aux.w = (int)((float)aux.w * wDiff);
	aux.h = (int)((float)aux.h * hDiff);

	if (lookingRight) {
		texture->render(recorteSheet, aux);
	}
	else {
		texture->render(recorteSheet, aux, 0., nullptr, SDL_FLIP_HORIZONTAL);
	}
}

void AnimationManager::render(SDL_Rect* camera, const Vector2D& offset)
{
	SDL_Rect aux = dest;

	float wDiff = (float)ent->GetManager()->GetActualWidth() / (float)camera->w;
	float hDiff = (float)ent->GetManager()->GetActualHeight() / (float)camera->h;

	aux.x -= camera->x;
	aux.x = (int)((float)aux.x * wDiff);

	aux.y -= camera->y;
	aux.y = (int)((float)aux.y * hDiff);

	aux.w = (int)((float)aux.w * wDiff);
	aux.h = (int)((float)aux.h * hDiff);

	aux.x += (int)nearbyint(offset.getX());
	aux.y += (int)nearbyint(offset.getY());

	if (lookingRight) {
		texture->render(recorteSheet, aux);
	}
	else {
		texture->render(recorteSheet, aux, 0., nullptr, SDL_FLIP_HORIZONTAL);
	}

#ifdef _DEBUG

	SDL_SetRenderDrawColor(ent->GetManager()->GetSDLU()->renderer(), 255, 255, 0, 255);
	// SDL_RenderDrawRect(ent->GetManager()->GetSDLU()->renderer(), &aux);

#endif // _DEBUG


}

void AnimationManager::StartAnimation(std::string index)
{
	currentAnimIndex = index;
	//La animacion actual cambia a ser la nueva especificada
	currentAnim = &info.animations[index];

	//El indice será el primero que esté en los datos
	currIndex = currentAnim->iniSprite;

	currentState = 0;

	assert(currentAnim->keySprite.size() == currentAnim->keyFrame.size());

	//Calcula cuanto debe durar cada sprite para que...
	if (currentAnim->keySprite.size() == 0)
	{
		//Acabe a la vez que el movimiento
		framespSprite = (int)(currentAnim->totalFrames / currentAnim->totalSprites);

		//Inicializamos el contador
		cont = framespSprite + currentAnim->totalFrames % currentAnim->totalSprites;
	}
	else
	{
		//El key Sprite coincida con la hitbox
		framespSprite = (int)(currentAnim->keyFrame[0] / currentAnim->keySprite[0]);

		//Inicializamos el contador
		cont = framespSprite + (currentAnim->keyFrame[0] % currentAnim->keySprite[0]);
	}
}

int AnimationManager::GetAnimationDuration()
{
	return currentAnim->totalFrames;
}



