#include "Makt.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "Ball.h"
#include <fstream>
#include "../../../utils/CheckML.h"
#include <iostream>

Makt::Makt(FightManager* mngr, b2Vec2 pos, char input,int p) :
	Character(mngr, pos, input,p, 2.f, 3.5f)
{

	json js = ReadJson("resources/config/maketo.json");

	baseJump = jumpStr;
	ballJump = js["ballJump"];
	jumpStr = ballJump;

	baseSpeed = maxSpeed;
	ballSpeed = js["ballSpeed"];
	maxSpeed = ballSpeed;

	baseWeight = weight;
	ballWeight = js["ballWeight"];
	weight = ballWeight;

	ball = nullptr;

	//guardamos la textura
	texture = &sdl->images().at("makt");
	portrait = &sdl->images().at("maktSelect");

	anim = new AnimationManager(this, texture, spData);
}

Makt::~Makt()
{
}

void Makt::BasicNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (ball == nullptr) {
			anim->StartAnimation("basicNB");
		}
		else {
			anim->StartAnimation("basicN");
		}
		sdl->soundEffects().at("maktAtk0").play();
	}
	else if (frameNumber == attacks["basicN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * .6f),
			body->GetPosition().y,
			width * 1.8f,
			height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicN"], 3, OnHitData(12, false, false)));
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicForward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		if (ball == nullptr) {
			anim->StartAnimation("basicFB");
		}
		else {
			anim->StartAnimation("basicF");
		}
		sdl->soundEffects().at("maktAtk1").play();

		body->SetLinearVelocity(b2Vec2(dir * 30, body->GetLinearVelocity().y));
	}
	else if (frameNumber == attacks["basicF"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir),
			body->GetPosition().y,
			width * 1.4f,
			height);

		attackData aaa = attacks["basicF"];

		if (!onGround)
		{
			aaa.direction.y = -10;
			aaa.direction.Normalize();
		}

		hitboxes.push_back(new Hitbox(hitbox, aaa, 5, OnHitData(20, false, false)));
	}
	else if (frameNumber == attacks["basicF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		if (ball == nullptr) {
			anim->StartAnimation("basicUB");
		}
		else {
			anim->StartAnimation("basicU");
		}
		sdl->soundEffects().at("maktAtk2").play();
	}
	else if (frameNumber == attacks["basicU"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * .2f),
			body->GetPosition().y - height * .5f,
			width * 1.8f,
			height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 5, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (ball == nullptr) {
			anim->StartAnimation("basicDB");
		}
		else {
			anim->StartAnimation("basicD");
		}
		sdl->soundEffects().at("maktAtk3").play();
	}
	else if (frameNumber == attacks["basicD"].startUp)
	{
		SDL_Rect sweetspot = manager->GetSDLCoors(
				body->GetPosition().x + (dir * width),
				body->GetPosition().y + height * .4f,
				width,
				height * 0.5f);

		hitboxes.push_back(new Hitbox(sweetspot, attacks["basicDS"], 3, OnHitData(16, false, false)));

		SDL_Rect sourspot = manager->GetSDLCoors(
			body->GetPosition().x - (dir * width * 0.5f),
			body->GetPosition().y + height * .4f,
			width * 2,
			height * 0.5f);

		hitboxes.push_back(new Hitbox(sourspot, attacks["basicD"], 3, OnHitData(10, false, false)));

	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::SpecialNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (ball != nullptr)
		{
			ChangeMove([this](int f) { BallPickUp(f); });
			return;
		}
		moving = false;
		anim->StartAnimation("especialN");
		sdl->soundEffects().at("maktSpecN").play();
	}
	else if (frameNumber == attacks["specialN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.2f),
			body->GetPosition().y,
			width * 1.7f,
			height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialN"], 8, OnHitData(30, false, false)));
	}
	else if (frameNumber >= attacks["specialN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
	
}

void Makt::SpecialForward(int frameNumber)
{

	if (frameNumber == 0)
	{
		if (ball != nullptr)
		{
			ChangeMove([this](int f) { BallPickUp(f); });
			return;
		}
		moving = false;
		anim->StartAnimation("especialLEntrada");
		sdl->soundEffects().at("maktSpecS").play();
	}
	
	else if (frameNumber >= attacks["specialL"].startUp)
	{
		if (frameNumber == attacks["specialL"].startUp) {
			anim->StartAnimation("especialLHold");
		}
		if(!input->special() || release) {
			if (!release) {		
				frameRelease = frameNumber;
			}
			release = true;
			if (frameNumber == frameRelease) {
				anim->StartAnimation("especialLSalida");
			}
			else if (frameNumber == frameRelease + 5) {
				release = false;
				ThrowBall(attacks["specialL"], frameNumber);
				ChangeMove([this](int f) {ThrowRecover(f); });
			}
			
		}		
	}
}

void Makt::SpecialUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (ball != nullptr)
		{
			ChangeMove([this](int f) { BallPickUp(f); });
			return;
		}
		moving = false;
		anim->StartAnimation("especialUEntrada");
		sdl->soundEffects().at("maktSpecU").play();
	}
	else if (frameNumber == attacks["specialU"].startUp)
	{
		anim->StartAnimation("especialU");
		SDL_Rect hitbox = manager->GetSDLCoors(
			body,
			width * 1.2f,
			height * 1.2f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialU"], 20, Vector2D(-5 + (5*dir), -50), OnHitData(3, false, false)));

		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -60));
	}
	else if (frameNumber >= attacks["specialU"].totalFrames)
	{
		recovery = true;
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::SpecialDownward(int frameNumber)
{

	if (frameNumber == 0)
	{
		if (ball != nullptr)
		{
			ChangeMove([this](int f) { BallPickUp(f); });
			return;
		}
		moving = false;
		anim->StartAnimation("especialD");
		sdl->soundEffects().at("maktSpecD").play();
	}
	else if (frameNumber == attacks["specialD"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y + height * 0.2f,
			width * 3,
			height * 0.7f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialD"], 5, OnHitData(3, false, false)));
	}
	else if (frameNumber >= attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::ThrowRecover(int frameNumber)
{
	if (frameNumber >= attacks["specialL"].totalFrames)
	{
		currentMove = nullptr;
	}
}

void Makt::BallPickUp(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		//sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["pickBall"].startUp)
	{
		if (SDL_HasIntersection(&hurtbox, ball->GetHurtbox())) {
			anim->StartAnimation("ballPick");
			RecoveredBall();
		}
	}
	else if (frameNumber >= attacks["pickBall"].totalFrames)
	{
		currentMove = nullptr;
	}
}

void Makt::update()
{
	// Para probar mandos
	//input->controllerTest();

	updateParticles();

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
	{
		stun--;
		if (!recovery) recovery = true;
	}


	if (stun > 0) {
		if (ball == nullptr) {
			if (anim->CurrentAnimation() != "stunB")
				anim->StartAnimation("stunB");
		}
		else {
			if (anim->CurrentAnimation() != "stun")
				anim->StartAnimation("stun");
		}

		if (input->right())
		{
			body->ApplyLinearImpulseToCenter({ 5, 0 }, true);
		}
		if (input->left())
		{
			body->ApplyLinearImpulseToCenter({ -5, 0 }, true);
		}

		currentMove = nullptr;
	}
	else
	{
		if (currentMove == nullptr || (currentMove != nullptr && !onGround))
		{
			if (input->right() && input->left())
			{
				speed = 0;
			}
			else
			{

				if (input->right())
				{
					if (currentMove == nullptr)
						dir = 1;

					if (speed < 1)
						AddParticle(new Particle(Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, "run", this));

					speed = maxSpeed;
				}
				if (input->left())
				{
					if (currentMove == nullptr)
						dir = -1;

					if (speed > -1)
						AddParticle(new Particle(Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, "run", this));

					speed = -maxSpeed;
				}
			}
		}

	}
	if (!recovery) {
		if (ball == nullptr) {
			if (anim->CurrentAnimation() != "dashB")
				anim->StartAnimation("dashB");
		}
		else {
			if (anim->CurrentAnimation() != "dash")
				anim->StartAnimation("dash");
		}

	}

	if (speed > 4)
		speed -= 4;
	else if (speed < -4)
		speed += 4;
	else
		speed = 0;

	if (currentMove == nullptr /*|| currentMove == Taunt())*/ && stun == 0 && recovery)
	{

		// Ataque con A (provisional)

		if (input->basic())
		{
			sdl->soundEffects().at(codeName + "Steps").haltChannel();

			if (input->up()) //básico arriba
			{
				StartMove([this](int f) { BasicUpward(f); });
			}
			else if (input->down()) //básico abajo
			{
				StartMove([this](int f) { BasicDownward(f); });
			}
			else if (input->right() || input->left()) //básico en movimiento
			{
				StartMove([this](int f) { BasicForward(f); });
			}
			else //básico estático
			{
				StartMove([this](int f) { BasicNeutral(f); });
			}

			manager->MoveToFront(this);

		}

		// Ataque con B (provisional)
		if (input->special())
		{
			sdl->soundEffects().at(codeName + "Steps").haltChannel();


			if (input->up()) //especial arriba
			{
				StartMove([this](int f) { SpecialUpward(f); });
			}
			else if (input->down()) //especial abajo
			{
				StartMove([this](int f) { SpecialDownward(f); });
			}
			else if (input->right() || input->left()) //especial en movimiento
			{
				StartMove([this](int f) { SpecialForward(f); });
			}
			else //especial estático
			{
				StartMove([this](int f) { SpecialNeutral(f); });
			}

			manager->MoveToFront(this);

		}

		//Escudo
		if (input->down() && onGround && shieldCounter > (maxShield / 3) && (body->GetLinearVelocity().y > -0.1f && body->GetLinearVelocity().y < 0.1f)) {

			StartMove([this](int f) { StartShield(f); });
			body->SetLinearVelocity(b2Vec2(0, 0));

		}
		else if (input->down() && !onGround)
		{
			StartMove([this](int f) { Dash(f); });
		}

		// salto
		if (input->up() && !(jumpCounter <= 0 || !jumpCooldown))
		{
			StartMove([this](int f) { StartJump(f); });
		}

		if (!GetGround())
		{
			if (ball == nullptr) {
				if (body->GetLinearVelocity().y > 0.01f && anim->CurrentAnimation() != "airborneB")
					anim->StartAnimation("airborneB");
			}
			else {
				if (body->GetLinearVelocity().y > 0.01f && anim->CurrentAnimation() != "airborne")
					anim->StartAnimation("airborne");
			}
		}
		else
		{
			if (speed > 0.1f || speed < -0.1f)
			{
				if (ball == nullptr) {
					if (anim->CurrentAnimation() != "runB")
						anim->StartAnimation("runB");
				}
				else {
					if (anim->CurrentAnimation() != "run")
						anim->StartAnimation("run");
				}
				sdl->soundEffects().at(codeName + "Steps").play();

			}
			//frenarse
			else
			{
				if (ball == nullptr) {
					if (anim->CurrentAnimation() != "idleB")
						anim->StartAnimation("idleB");
				}
				else {
					if (anim->CurrentAnimation() != "idle")
						anim->StartAnimation("idle");
				}
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
				for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
					auto fix = f->GetFilterData();
					fix.maskBits = 2; // Quita la colisión con la plataforma momentáneamente
					f->SetFilterData(fix);
				}
				fall = 0;
			}
		}
		if (input->taunt() && onGround)
		{
			sdl->soundEffects().at(codeName + "Taunt").play();

			StartMove([this](int f) { Taunt(f); });
		}
	}

	//else sdl->soundEffects().at(codeName + "Steps").haltChannel();

	if (input->down() && body->GetFixtureList()->GetFilterData().maskBits != 2) down = true; // Marca que se ha pulsado abajo (para el tema de bajar plataformas)

	if (reactivateColl > 0) reactivateColl--;
	if (reactivateColl == 0 && body->GetFixtureList()->GetFilterData().maskBits == 2) { // Tras medio segundo reactiva colisión jugador-plataformas
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			auto fix = f->GetFilterData();
			fix.maskBits = 2 | 4;
			f->SetFilterData(fix);
		}
	}
	//para recuperar escudo
	if (!shield && shieldCounter < maxShield)
	{
		shieldCounter++;
	}
	else if (shield)
	{

		shieldCounter -= 2;
	}

	//Chequeo de tierra
	if (GetGround())
	{
		jumpCounter = maxJumps;
		if (!recovery) recovery = true;
	}
	//chequeo doble salto
	if (!input->up() && !jumpCooldown)
	{
		jumpCooldown = true;
	}

	if (stun == 0)
		body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));


	//Si hay un movimiento en ejecucion lo continuamos...
	if (currentMove != nullptr)
	{
		//ejecuta el ataque guardado en la variable
		(currentMove)(moveFrame);
		//Actualiza el frame actual del movimiento
		moveFrame++;
	}

	// Efectos de estado
	if (efEstado != none && stateCont < stateDur)
	{
		stateCont++;
		if ((efEstado == fire || efEstado == wElectric) && stateCont % 60 == 0)
		{
			Elements();
		}
	}
	else
	{
		if (efEstado == water)
		{
			maxSpeed += ralentizar;
			ralentizar = 0;
		}
		stateCont = 0;
		statePower = 0;
		efEstado = none;
	}

	anim->update();

	hurtbox.x = manager->b2ToSDLX(body, width);
	hurtbox.y = manager->b2ToSDLY(body, height);

	if (!SDL_HasIntersection(&hurtbox, manager->GetDeathZone()))
	{
		OnDeath();
	}

}

bool Makt::GetHit(attackData a, Entity* attacker)
{
	if (Character::GetHit(a, attacker))
	{
		if (ball == nullptr && !shield) {
			anim->StartAnimation("stunB");
		}
		else if(!shield)
		{
			anim->StartAnimation("stun");
		}
		anim->update();
		return true;
	}
	return false;
}

void Makt::StartJump(int frameNumber)
{
	if (jumpCounter <= 0 || !jumpCooldown)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (frameNumber < 3)
	{
		if (ball == nullptr) {
			anim->StartAnimation("jumpChargeB");
		}
		else {
			anim->StartAnimation("jumpCharge");
		}
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

		if (input->special())
		{
			ChangeMove([this](int f) { SpecialUpward(f); });
			moveFrame = -1;
		}
		else if (input->basic())
		{
			ChangeMove([this](int f) { BasicUpward(f); });
			moveFrame = -1;
		}
	}
	else if (frameNumber >= 4)
	{
		sdl->soundEffects().at(codeName + "Steps").haltChannel();

		if (ball == nullptr) {
			anim->StartAnimation("jumpB");
		}
		else {
			anim->StartAnimation("jump");
		}
		if (!GetGround())
		{
			sdl->soundEffects().at("jump1").play();

			jumpCounter--;
		}
		else sdl->soundEffects().at("jump0").play();

		jumpCooldown = false;
		body->SetLinearVelocity(b2Vec2(speed, 0));

		if (input->basic())
		{

			if (input->up()) //básico arriba
			{
				ChangeMove([this](int f) { BasicUpward(f); });
			}
			else if (input->down()) //básico abajo
			{
				ChangeMove([this](int f) { BasicDownward(f); });
			}
			else if (input->right() || input->left()) //básico en movimiento
			{
				ChangeMove([this](int f) { BasicForward(f); });
			}
			else //básico estático
			{
				ChangeMove([this](int f) { BasicNeutral(f); });
			}

			manager->MoveToFront(this);
			body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);

		}
		else if (input->special())
		{

			if (input->up()) //especial arriba
			{
				ChangeMove([this](int f) { SpecialUpward(f); });
			}
			else if (input->down()) //especial abajo
			{
				ChangeMove([this](int f) { SpecialDownward(f); });
			}
			else if (input->right() || input->left()) //especial en movimiento
			{
				ChangeMove([this](int f) { SpecialForward(f); });
			}
			else //especial estático
			{
				ChangeMove([this](int f) { SpecialNeutral(f); });
			}

			manager->MoveToFront(this);
			body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);

		}
		else
		{
			if (input->up())
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
			else
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);
		}

		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::StartShield(int frameNumber)
{
	if (frameNumber == 1)
	{
		sdl->soundEffects().at("shield").play();

		if (ball == nullptr) {
			anim->StartAnimation("shieldB");
		}
		else {
			anim->StartAnimation("shield");
		}
		shield = true;
	}
	if (!input->down() || shieldCounter <= 0)
	{
		ChangeMove([this](int f) { EndShield(f); });
	}
	if (input->basic())
	{
		shield = false;
		ChangeMove([this](int f) { BasicDownward(f); });
	}
	else if (input->special())
	{
		shield = false;
		ChangeMove([this](int f) { SpecialDownward(f); });
	}
}
void Makt::EndShield(int frameNumber)
{
	anim->StartAnimation("idle");
	currentMove = nullptr;
	moveFrame = -1;
	shield = false;
}

void Makt::Dash(int frameNumber)
{

	switch (frameNumber)
	{
	case 0:
		if (ball == nullptr) {
			anim->StartAnimation("dashB");
		}
		else {
			anim->StartAnimation("dash");
		}
		dash = true;
		body->SetLinearVelocity(b2Vec2(0, 20));
		break;
	case 60:
		dash = false;
		currentMove = nullptr;
		if (ball == nullptr) {
			anim->StartAnimation("idleB");
		}
		else {
			anim->StartAnimation("idle");
		}
		break;
	}
	if (onGround)
	{
		dash = false;
		currentMove = nullptr;
		if (ball == nullptr) {
			anim->StartAnimation("idleB");
		}
		else {
			anim->StartAnimation("idle");
		}
	}
}

void Makt::Taunt(int frameNumber)
{
	if (frameNumber == 1)
	{
		anim->StartAnimation("taunt");
	}
	if (input->right())
	{
		speed = maxSpeed;
		dir = 1;
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (input->left())
	{
		speed = -maxSpeed;
		dir = -1;
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (input->right() && input->left())
	{
		speed = 0;
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (input->basic())
	{

		if (input->up()) //básico arriba
		{
			ChangeMove([this](int f) { BasicUpward(f); });
		}
		else if (input->down()) //básico abajo
		{
			ChangeMove([this](int f) { BasicDownward(f); });
		}
		else if (input->right() || input->left()) //básico en movimiento
		{
			ChangeMove([this](int f) { BasicForward(f); });
		}
		else //básico estático
		{
			ChangeMove([this](int f) { BasicNeutral(f); });
		}

		manager->MoveToFront(this);

	}

	// Ataque con B (provisional)
	if (input->special())
	{

		if (input->up()) //especial arriba
		{
			ChangeMove([this](int f) { SpecialUpward(f); });
		}
		else if (input->down()) //especial abajo
		{
			ChangeMove([this](int f) { SpecialDownward(f); });
		}
		else if (input->right() || input->left()) //especial en movimiento
		{
			ChangeMove([this](int f) { SpecialForward(f); });
		}
		else //especial estático
		{
			ChangeMove([this](int f) { SpecialNeutral(f); });
		}

		manager->MoveToFront(this);

	}

	if (input->down() && onGround && shieldCounter > (maxShield / 3) && (body->GetLinearVelocity().y > -0.1f && body->GetLinearVelocity().y < 0.1f)) {

		ChangeMove([this](int f) { StartShield(f); });
		body->SetLinearVelocity(b2Vec2(0, 0));

	}
	// salto
	if (input->up() && !(jumpCounter <= 0 || !jumpCooldown))
	{
		ChangeMove([this](int f) { StartJump(f); });
	}

	else if (frameNumber == spData.animations["taunt"].totalFrames)
	{
		if (ball == nullptr) {
			anim->StartAnimation("idleB");
		}
		else {
			anim->StartAnimation("idle");
		}
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::drawHUD(int numOfPlayer)
{
	Character::drawHUD( numOfPlayer);

}

void Makt::RecoveredBall()
{
	if (ball->PickUp())
	{
		jumpStr = ballJump;
		maxSpeed = ballSpeed;
		weight = ballWeight;
		ball = nullptr;
	}
}

void Makt::ThrowBall(attackData force, int timeHeld)
{
	if (timeHeld > 200)
	{
		timeHeld = 200;
	}

	jumpStr = baseJump;
	maxSpeed = baseSpeed;
	weight = baseWeight;

	attackData aux = force;

	aux.damage += timeHeld / 10;
	aux.base += timeHeld / 10;

	ball = new MaktBall(manager, b2Vec2( body->GetPosition().x + dir, body->GetPosition().y ), aux, b2Vec2(dir, 0), respawnPos);
	manager->AddEntity(ball);
	ball->SetOponents(oponents);
}
