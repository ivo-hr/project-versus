#include "Character.h"
#include "Utils/AnimationManager.h"
#include "Utils/InputConfig.h"
#include "Utils/Particle.h"
#include "../json/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;


void Character::ReadJson(std::string filename)
{
	std::ifstream file(filename);
	json jsonFile;
	file >> jsonFile;


	// variables
	codeName = jsonFile["codeName"];
	weight = jsonFile["weight"];
	maxSpeed = jsonFile["maxSpeed"];
	speed = jsonFile["speed"];
	maxJumps = jsonFile["maxJumps"];
	jumpStr = jsonFile["jumpStr"];
	onGround = jsonFile["onGround"];
	shield = jsonFile["shield"];
	maxShield = jsonFile["maxShield"];
	jumpCounter = maxJumps;
	damageTaken = 0;
	shieldCounter = maxShield;
	jumpCooldown = true;


	//Aqui defino las caracteristicas de cada hitbox (podriamos hacerlo dentro de cada metodo, y vendria de json)(tambien podríamos poner framedata)


	attackData aux;
	auto aData = jsonFile["attacksData"];
	assert(aData.is_array());

	for (uint16 i = 0u; i < aData.size(); i++) {

		aux.direction = b2Vec2(aData[i]["b2vecX"], aData[i]["b2vecY"]);
		aux.direction.Normalize();
		aux.base = aData[i]["base"];
		aux.damage = aData[i]["damage"];
		aux.multiplier = aData[i]["multiplier"];
		aux.startUp = aData[i]["startUp"];
		aux.totalFrames = aData[i]["totalFrames"];

		attacks.insert({ aData[i]["id"], aux });
	}


	//Datos para las animaciones (tendrá que venir de json claramente solo hay tres y ya ocupan 37 lineas xd)
	auto sData = jsonFile["spData"];
	//Mirando a la derecha

	spData.leftOffset = sData["leftOffset"];		//Pixeles en sprite que se dibujaran fuera de la hurtbox a la izquierda
	spData.upOffset = sData["upOffset"];
	spData.sizeXOffset = sData["sizeXOffset"];	//Cuantos pixeles en X NO estan dentro de la hurtbox
	spData.sizeYOffset = sData["sizeYOffset"];

	spData.spritesInX = sData["spritesInX"];
	spData.spritesInY = sData["spritesInY"];

	animationData auxAnim;
	auto animData = jsonFile["animationData"]["anim"];
	assert(animData.is_array());

	for (uint16 i = 0u; i < animData.size(); i++) {

		auxAnim.iniSprite = animData[i]["iniSprite"];
		auxAnim.totalSprites = animData[i]["totalSprites"];
		auxAnim.keySprite = animData[i]["keySprite"];

		if (animData[i]["attack"] != "")
		{
			auxAnim.hitboxFrame = attacks[animData[i]["attack"]].startUp;
			auxAnim.totalFrames = attacks[animData[i]["attack"]].totalFrames;
		}
		else
		{
			auxAnim.keySprite = -1;
			auxAnim.hitboxFrame = -1;
			auxAnim.totalFrames = animData[i]["totalFrames"];
		}
		auxAnim.loop = animData[i]["loop"];

		spData.animations.insert({ animData[i]["id"], auxAnim });
	}
}

Character::Character(FightManager* manager, Vector2D* pos, char input, float w, float h) :
	Entity(manager, pos, w, h)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	stun = 0;
	dash = false;
	lives = 3;
	this->input = new InputConfig(input);
}

Character::~Character()
{

}

void Character::update()
{

	if (!alive)
	{
		respawnFrames--;
		if (respawnFrames == 0)
		{
			Respawn();
			respawnFrames = 150;
		}
		return;
	}

	if (stun > 0)
		stun--;

	if (currentMove == nullptr && stun == 0)
	{

		speed -= 4;

		if (input->right())
		{
			speed = maxSpeed;
			dir = 1;
		}
		if (input->left())
		{
			speed = -maxSpeed;
			dir = -1;
		}
		if (input->right() && input->left())
		{
			speed = 0;
		}

		// Ataque con A (provisional)
		
		if (input->basic())
		{

			if (input->right() || input->left()) //básico en movimiento
			{
				currentMove = [this](int f) { BasicForward(f); };
			}
			else if (input->down()) //básico abajo
			{
				currentMove = [this](int f) { BasicDownward(f); };
			}
			else if (input->up()) //básico arriba
			{
				currentMove = [this](int f) { BasicUpward(f); };
			}
			else //básico estático
			{
				currentMove = [this](int f) { BasicNeutral(f); };
			}

			manager->MoveToFront(this);

		}

		// Ataque con B (provisional)
		if (input->special())
		{

			if (input->right() || input->left()) //especial en movimiento
			{
				currentMove = [this](int f) { SpecialForward(f); };
			}
			else if (input->down()) //especial abajo
			{
				currentMove = [this](int f) { SpecialDownward(f); };
			}
			else if (input->up()) //especial arriba
			{
				currentMove = [this](int f) { SpecialUpward(f); };
			}
			else //especial estático
			{
				currentMove = [this](int f) { SpecialNeutral(f); };
			}

			manager->MoveToFront(this);

		}

		//Escudo
		if (input->down() && onGround && shieldCounter > (maxShield/3) && (body->GetLinearVelocity().y > -0.1f && body->GetLinearVelocity().y < 0.1f)) {

			currentMove = [this](int f) { StartShield(f); };
			body->SetLinearVelocity(b2Vec2(0, 0));

		}
		else if (input->down() && !onGround)
		{
			currentMove = [this](int f) { Dash(f); };
		}


		// salto
		if (input->up()) 
		{
			currentMove = [this](int f) { StartJump(f); };
		}
		
		//dash
		if () {

		}

		if (!GetGround())
		{
			if (anim->CurrentAnimation() != "airborne")
				anim->StartAnimation("airborne");
		}
		else
		{
			if (moving)
			{
				if (anim->CurrentAnimation() != "run")
					anim->StartAnimation("run");

				sdl->soundEffects().at(codeName + "Steps").play();
			}
			//frenarse
			else
			{
				if (anim->CurrentAnimation() != "idle")
					anim->StartAnimation("idle");
			}
		}

		// bajar plataformas
		if (down && input->downReleased()) {
			down = false;
			fall = maxFallCount; // Activa contador para reconocer el bajar plataformas
		}

		if (onGround && fall > 0) {
			fall--;
			if (input->down()) { // Va a atravesar la plataforma
				reactivateColl = maxFallCount; 
				b2Filter f = body->GetFixtureList()->GetFilterData();
				f.maskBits = 2; // Quita la colisión con la plataforma momentáneamente
				body->GetFixtureList()->SetFilterData(f);
				fall = 0;
			}
		}
	}

	if (input->down() && body->GetFixtureList()->GetFilterData().maskBits != 2) down = true; // Marca que se ha pulsado abajo (para el tema de bajar plataformas)

	if (reactivateColl > 0) reactivateColl--;
	if (reactivateColl == 0 && body->GetFixtureList()->GetFilterData().maskBits == 2) { // Tras medio segundo reactiva colisión jugador-plataformas
		b2Filter f = body->GetFixtureList()->GetFilterData();
		f.maskBits = 2 | 4;
		body->GetFixtureList()->SetFilterData(f);
	}

	if (stun > 0) {
		if (anim->CurrentAnimation() != "stun")
			anim->StartAnimation("stun");
	}
	//para recuperar escudo
	if (!shield && shieldCounter < maxShield)
	{
		shieldCounter++;
	}
	else if (shield)
	{

		shieldCounter-=2;
	}

	//Chequeo de tierra
	if (GetGround())
	{
		jumpCounter = maxJumps;
	}
	//chequeo doble salto
	if (!input->up() && !jumpCooldown)
	{
		jumpCooldown = true;
	}

	//frenarse
	if (!moving)
	{
		if (speed > 4)
			speed -= 4;
		else if (speed < -4)
			speed += 4;
		else
			speed = 0;
	}

	//Que se mueva si no esta haciendo un ataque ya
	if (currentMove == nullptr && stun == 0)
		body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));


	//Si hay un movimiento en ejecucion lo continuamos...
	if (currentMove != nullptr)
	{
		//ejecuta el ataque guardado en la variable
		(currentMove)(moveFrame);
		//Actualiza el frame actual del movimiento
		moveFrame++;
	}

	anim->update();
	Entity::update();

}

void Character::draw()
{
	//xd

	if (!alive) return;

	Entity::draw();
	anim->render();

	//if (debug)

	if (shield)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 0, 255, 255);
	}
	else if (dash)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 255, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 255, 0, 255);
	}
	SDL_RenderDrawRect(sdl->renderer(), &hurtbox);
}


bool Character::GetHit(attackData a, Entity* attacker)
{
	
	if (shield)
	{
		//Actualiza el da�o
		damageTaken += (int)(a.damage * 0.4f);
		return true;
	}
	if (dash)
	{
		return false;
	}
	else if (!shield && !dash)
	{
		currentMove = nullptr;
		moveFrame = -1;
		anim->StartAnimation("stun");
		anim->update();
		float recoil = (a.base + ((damageTaken * a.multiplier) / (weight * .2f)));

		if (a.base >= 0)
		{
			stun = recoil / 1.8f;
		}

		//Actualiza el da�o
		damageTaken += a.damage;
		
		b2Vec2 aux = a.direction;

		if (recoil > 100)
		{
			manager->KillingBlow(Vector2D(
				manager->ToSDL(body->GetPosition().x),
				manager->ToSDL(body->GetPosition().y)));
		}

		aux *= recoil;
		aux.y *= -1;
		aux.x *= attacker->GetDir();

		//Produce el knoback..
		body->SetLinearVelocity(aux);

		return true;
	}

}
void Character::StartJump(int frameNumber)
{
	if (jumpCounter <= 0 || !jumpCooldown)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (frameNumber < 4)
	{
		if (input->special())
		{
			currentMove = [this](int f) { SpecialUpward(f); };
			moveFrame = -1;
		}
		else if (input->basic())
		{
			currentMove = [this](int f) { BasicUpward(f); };
			moveFrame = -1;
		}
	}
	else
	{
		if (!GetGround())
		{
			sdl->soundEffects().at("jump1").play();

			jumpCounter--;
		}
		else sdl->soundEffects().at("jump0").play();

		jumpCooldown = false;
		body->SetLinearVelocity(b2Vec2(speed, 0));
		body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);

		currentMove = nullptr;
		moveFrame = -1;
	}
}
void Character::StartShield(int frameNumber)
{
	if (frameNumber == 1)
	{
		sdl->soundEffects().at("shield").play();

		anim->StartAnimation("shield");
		shield = true;
	}
	if (!input->down() || shieldCounter <= 0 )
	{
		currentMove = [this](int f) { EndShield(f); };
	}
	if (input->basic())
	{
		moveFrame = -1;
		shield = false;
		currentMove = [this](int f) { BasicDownward(f); };
	}
	else if (input->special())
	{
		moveFrame = -1;
		shield = false;
		currentMove = [this](int f) { SpecialDownward(f); };
	}
}
void Character::EndShield(int frameNumber)
{
	anim->StartAnimation("idle");
	currentMove = nullptr;
	moveFrame = -1;
	shield = false;
}

void Character::Dash(int frameNumber)
{

	switch (frameNumber)
	{
	case 0:
		anim->StartAnimation("dash");
		dash = true;
		body->SetLinearVelocity(b2Vec2(0, 20));
		break;
	case 60:
		dash = false;
		currentMove = nullptr;
		moveFrame = -1;
		anim->StartAnimation(0);
		break;
	}
	if (onGround)
	{
		dash = false;
		currentMove = nullptr;
		moveFrame = -1;
		anim->StartAnimation("idle");
	}
}

SDL_Rect* Character::GetHurtbox()
{
	return &hurtbox;
}

void Character::OnDeath()
{
	body->SetAwake(false);
	alive = false;
	lives--;
	currentMove = nullptr;
	moveFrame = 0;
	moving = false;
	shield = false;
	dash = false;
}

void Character::Respawn()
{
	if (lives <= 0) {
		return;
	}
	body->SetAwake(true);

	std::cout << "Vidas restantes: " << lives << "\n";

	body->SetTransform({ respawnPos.getX(), respawnPos.getY() }, 0);
	body->SetLinearVelocity({ 0, 0 }); // resetea la velocidad
	speed = 0;

	alive = true;
	damageTaken = 0;
	moving = false;

	currentMove = nullptr;
	moveFrame = 0;

	anim->StartAnimation("idle");
}


