#include "CharacterZero.h"
#include "../../Utils/AnimationManager.h"

CharacterZero::CharacterZero(FightManager* mngr, b2Vec2 pos, char input,int p) : Character(mngr, pos, input,p)
{
	//importamos json del personaje
	
	ReadJson("resources/config/zero.json");

	//guardamos la textura
	texture = &sdl->images().at("zero");
	portrait = &sdl->images().at("zeroSelect");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

	anim = new AnimationManager(this, texture, spData);
}

CharacterZero::~CharacterZero()
{

}

void CharacterZero::draw()
{
	Character::draw();
}


void CharacterZero::BasicNeutral(int frameNumber)
{

	//Dependiendo del frame en el que esté, hara una cosa u otra..

	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroSpecN").play();
		//Empieza el ataque :v
		anim->StartAnimation("big");
		break;
	case 56:
	{
		//Al frame 90, crea un rect y si el oponente colisiona con ello...
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w /= 2;
		hitbox.h /= 2;

		hitbox.x += hitbox.w / 2;

		hitbox.y += hitbox.h / 2;

		hitbox.x += dir * 60;
		
		hitboxes.push_back(new Hitbox(hitbox, 
			attacks["fuerte"],
			1, OnHitData(20, false, false)));

	}
	break;
	case 100:

		//Al ultimo frame...

		//SDL_DestroyRenderer(sdl->renderer());
		//char a = 'a';
		//SDL_ShowSimpleMessageBox(1, &a, &a, SDL_CreateWindow(&a, 10, 10, 10, 10, 1));
		//SDL_DestroyWindow(sdl->window());
		//SDL_Quit();


		//Vacia current move para que Character sepa que ha acabado
		currentMove = nullptr;

		//Reinicia moveFrame para el siguiente
		moveFrame = -1;

		break;
	}
}



//Lo mismo que el de arriba pero mas lento y fuerte xd
void CharacterZero::SpecialNeutral(int frameNumber)
{
	
	switch (frameNumber)
	{
	case 0:
		anim->StartAnimation("small");
		break;
	case 12:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 30;

		hitboxes.push_back(new Hitbox(hitbox, 
			attacks["debil"], 
			1));

	}
	break;
	case 20:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}

void CharacterZero::drawHUD(int w, int h, int numOfPlayer , int screenadjust)
{
	Character::drawHUD(w, h, numOfPlayer, screenadjust);

}
