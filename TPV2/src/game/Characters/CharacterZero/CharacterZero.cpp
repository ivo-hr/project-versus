#include "CharacterZero.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

CharacterZero::CharacterZero(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input)
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
	shieldCounter = maxShield;
	jumpCooldown = true;
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

		spData.animations.insert({ aData[i]["id"], aux });
	}
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
		sdl->soundEffects().at("zeroBigHit").play();
		//Empieza el ataque :v
		anim->StartAnimation("big");
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
		
		hitboxes.push_back(new Hitbox(hitbox, ataqueFuerte, 1, OnHitData(20, false, false)));

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

		hitboxes.push_back(new Hitbox(hitbox, ataqueDebil, 1));

	}
	break;
	case 20:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}
