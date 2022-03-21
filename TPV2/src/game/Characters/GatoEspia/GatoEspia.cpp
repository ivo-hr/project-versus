#include "GatoEspia.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;
GatoEspia::GatoEspia(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input)
{

	//importamos json del personaje
	std::ifstream file("resources/config/zero.json");
	json jsonFile;
	file >> jsonFile;

	//guardamos la textura
	texture = &sdl->images().at("zero");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");


	//Aqui defino las caracteristicas de cada hitbox (podriamos hacerlo dentro de cada metodo, y vendria de json)(tambien podríamos poner framedata)
	auto AF = jsonFile["Ataques"]["Fuerte"];
	auto AD = jsonFile["Ataques"]["Debil"];

	ataqueFuerte.direction = b2Vec2(AF["b2vecX"], AF["b2vecY"]);
	ataqueFuerte.direction.Normalize();
	ataqueFuerte.base = AF["base"];
	ataqueFuerte.damage = AF["damage"];
	ataqueFuerte.multiplier = AF["multiplier"];

	ataqueDebil.direction = b2Vec2(AD["b2vecX"], AD["b2vecY"]);
	ataqueDebil.direction.Normalize();
	ataqueDebil.base = AD["base"];
	ataqueDebil.damage = AD["damage"];
	ataqueDebil.multiplier = AD["multiplier"];

	// variables
	weight = jsonFile["weight"];
	damageTaken = jsonFile["damageTaken"];
	maxSpeed = jsonFile["maxSpeed"];
	speed = jsonFile["speed"];
	maxJumps = jsonFile["maxJumps"];
	jumpStr = jsonFile["jumpStr"];
	jumpCounter = maxJumps;
	onGround = jsonFile["onGround"];
	shield = jsonFile["shield"];
	maxShield = jsonFile["maxShield"];

	//Datos para las animaciones (tendrá que venir de json claramente solo hay tres y ya ocupan 37 lineas xd)
	auto sData = jsonFile["spData"];
	//Mirando a la derecha

	spData.leftOffset = sData["leftOffset"];		//Pixeles en sprite que se dibujaran fuera de la hurtbox a la izquierda
	spData.upOffset = sData["upOffset"];
	spData.sizeXOffset = sData["sizeXOffset"];	//Cuantos pixeles en X NO estan dentro de la hurtbox
	spData.sizeYOffset = sData["sizeYOffset"];

	spData.spritesInX = sData["spritesInX"];
	spData.spritesInY = sData["spritesInY"];

	animationData aux;
	auto aData = jsonFile["animationData"]["anim"];
	assert(aData.is_array());

	for (uint16 i = 0u; i < aData.size(); i++) {

		aux.iniSprite = aData[i]["iniSprite"];
		aux.totalSprites = aData[i]["totalSprites"];
		aux.keySprite = aData[i]["keySprite"];
		aux.hitboxFrame = aData[i]["hitboxFrame"];
		aux.totalFrames = aData[i]["totalFrames"];
		aux.loop = aData[i]["loop"];

		spData.animations.push_back(aux);
	}
	anim = new AnimationManager(this, texture, spData);
}

GatoEspia::~GatoEspia()
{

}

void GatoEspia::draw()
{
	Character::draw();
}

void GatoEspia::SpecialNeutral(int frameNumber)
{

	//Dependiendo del frame en el que esté, hara una cosa u otra..

	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroBigHit").play();
		//Empieza el ataque :v
		anim->StartAnimation(2);
		break;
		//No hace nada, esto es el cargar el puño
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

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		for (int i = 0; i < oponents.size(); i++)
		{
			if (SDL_HasIntersection(&hitbox, oponents[i]->GetHurtbox()))
			{
				//Le hace daño xddd
				if (oponents[i]->GetHit(ataqueFuerte, dir))
				{
					manager->HitLag(20);
				}
			}
		}
	}
	break;
	case 100:

		//Al ultimo frame...

		//Vacia current move para que Character sepa que ha acabado
		currentMove = nullptr;

		//Reinicia moveFrame para el siguiente
		moveFrame = -1;
		break;
	}
}

//Lo mismo que el de arriba pero mas rapido y debil xd
void GatoEspia::BasicNeutral(int frameNumber)
{
	
	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroSmolHit").play();
		anim->StartAnimation(1);
		break;
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
