#include "CharacterZero.h"

CharacterZero::CharacterZero(FightManager* mngr) : Character(mngr)
{
	texture = &sdl->images().at("makt");

	//Aqui defino las caracteristicas de cada hitbox (podriamos hacerlo dentro de cada metodo, y vendria de json)(tambien podríamos poner framedata)
	ataqueFuerte.damage = 20;
	ataqueFuerte.multiplier = 1.8f;

	ataqueDebil.damage = 5;
	ataqueDebil.multiplier = 0.2f;

	// variables
	damageTaken = 0;
	maxSpeed = 40;
	speed = 0;
	maxJumps = 1;
	jumpStr = 10000;
	jumpCounter = maxJumps;
	onGround = true;
}

CharacterZero::~CharacterZero()
{

}


void CharacterZero::BasicNeutral(int frameNumber)
{

	//Dependiendo del frame en el que esté, hara una cosa u otra..

	switch (frameNumber)
	{
	case 0:
		//Empieza el ataque :v
		//No hace nada, esto es el cargar el puño
		break;
	case 90:
	{

		//Al frame 90, crea un rect y si el oponente colisiona con ello...
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w /= 2;
		hitbox.h /= 2;

		hitbox.x += hitbox.w / 2;

		hitbox.y += hitbox.h / 2;

		hitbox.x += dir * 60;

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		for (int i = 0; i < oponents.size(); i++)
		{
			if (SDL_HasIntersection(&hitbox, oponents[i]->GetHurtbox()))
			{
				//Le hace daño xddd
				oponents[i]->GetHit(ataqueFuerte, dir);
			}
		}
	}
	break;
	case 160:

		//Al ultimo frame...

		//Vacia current move para que Character sepa que ha acabado
		currentMove = nullptr;

		//Reinicia moveFrame para el siguiente
		moveFrame = -1;
		break;
	}
}



//Lo mismo que el de arriba pero mas rapido y debil xd
void CharacterZero::SpecialNeutral(int frameNumber)
{
	switch (frameNumber)
	{
	case 12:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 30;

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		for (int i = 0; i < oponents.size(); i++)
		{
			if (SDL_HasIntersection(&hitbox, oponents[i]->GetHurtbox()))
			{
				//Le hace daño xddd
				oponents[i]->GetHit(ataqueDebil, dir);
			}
		}
	}
	break;
	case 20:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}
