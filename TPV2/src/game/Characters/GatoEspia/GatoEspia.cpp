#include "GatoEspia.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;
GatoEspia::GatoEspia(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input, 1.5f, 3.f)
{

	//importamos json del personaje
	std::ifstream file("resources/config/gato.json");
	json jsonFile;
	file >> jsonFile;

	//guardamos la textura
	texture = &sdl->images().at("blinkMaster");
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

GatoEspia::~GatoEspia()
{

}

void GatoEspia::draw()
{
	Character::draw();
}

//Lo mismo que el de arriba pero mas rapido y debil xd
void GatoEspia::BasicNeutral(int frameNumber)
{
	
	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroSmolHit").play();
		anim->StartAnimation("basicN");
		break;
	case 4:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 30;


		hitboxes.push_back(new Hitbox(hitbox, ataqueDebil, 2, OnHitData(5, false, false)));
	}
	break;
	case 15:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}
void GatoEspia::BasicForward(int frameNumber)
{

	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroSmolHit").play();//cambio
		anim->StartAnimation("basicF");//cambio
		break;
	case 12:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 50; // cambio

		body->SetLinearVelocity(b2Vec2(dir*20, 0));

		hitboxes.push_back(new Hitbox(hitbox, ataqueFuerte, 1, OnHitData(20, false, false)));
	}
	break;
	case 40:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}
void GatoEspia::BasicDownward(int frameNumber)
{
	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroBigHit").play();//cambio
		anim->StartAnimation("basicD");//cambio
		break;
	case 12:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body-30, width+60, height);

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

	}
	break;
	case 35:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}

void GatoEspia::BasicUpward(int frameNumber)
{
	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroBigHit").play();//cambio
		anim->StartAnimation("basicU");//cambio
		break;
	case 10:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body - 30, width + 60, height);

		hitbox.y -= 50;

		hitboxes.push_back(new Hitbox(hitbox, ataqueDebil, 1, OnHitData(20, false, false)));

	}
	break;
	case 20:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}

void GatoEspia::SpecialNeutral(int frameNumber)
{

	//Dependiendo del frame en el que esté, hara una cosa u otra..

	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroBigHit").play();
		//Empieza el ataque :v
		anim->StartAnimation("basicN");
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


		hitboxes.push_back(new Hitbox(hitbox, ataqueFuerte, 4, OnHitData(10, false, false)));

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

void GatoEspia::SpecialForward(int frameNumber)
{
	switch (frameNumber)
	{
	case 0:
	{
		//sdl->soundEffects().at("zeroBigHit").play();
		anim->StartAnimation("idle");
		moving = false;
		break;
	}
	break;
	case 2:
	{
		dash = true;
	}
	break;
	case 6:
	{
		body->SetTransform(body->GetPosition() + b2Vec2(dir * 10, 0),0);
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, 0 });
		dash = false;
	}
	break;
	case 7:
	{
		if (input->special())
		{
			currentMove = [this](int f) { TpAtack(f);};
			moveFrame = -1;
		}
	}
	break;
	case 8:


		//Al ultimo frame...

		//Vacia current move para que Character sepa que ha acabado
		currentMove = nullptr;

		//Reinicia moveFrame para el siguiente
		moveFrame = -1;
		break;
	}
}

void GatoEspia::TpAtack(int frameNumber)
{

	switch (frameNumber)
	{
	case 0:
	{
		//sdl->soundEffects().at("zeroBigHit").play();
		anim->StartAnimation("basicF");
		moving = false;
		break;
	}
	break;
	case 2:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitboxes.push_back(new Hitbox(hitbox, ataqueFuerte, 1, OnHitData(40, false, false)));
	}
	break;
	case 60:


		//Al ultimo frame...

		//Vacia current move para que Character sepa que ha acabado
		currentMove = nullptr;

		//Reinicia moveFrame para el siguiente
		moveFrame = -1;
		break;
	}

}


