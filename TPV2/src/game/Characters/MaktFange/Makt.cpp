#include "Makt.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "Ball.h"
#include <fstream>
#include "../../../utils/CheckML.h"
#include <iostream>

Makt::Makt(FightManager* mngr, b2Vec2 pos, char input, ushort p) :
	Character(mngr, pos, input,p, 2.f, 3.5f)
{

	spriteSheetData spData;

	json js = ReadJson("resources/config/maketo.json", spData);

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

	eyePos = { (float)hurtbox.w / 1.45f, (float)hurtbox.h / 20.f };

	animAddon = "B";
}

Makt::~Makt()
{
}

void Makt::BasicNeutral(ushort frameNumber)
{
	if (ballRecover > 0) {
		return;
	}
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN" + animAddon);
		sdl->soundEffects().at("maktAtk0").play();
	}
	else if (frameNumber == attacks["basicN"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicN"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicForward(ushort frameNumber)
{
	if (ballRecover > 0) {
		return;
	}
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

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
	else if (frameNumber == attacks["basicF"].keyFrames[0])
	{
		if (onGround)
			CreateHitBox(&attacks["basicF"].hitBoxes[0]);
		else
			CreateHitBox(&attacks["basicF"].hitBoxes[1]);
	}
	else if (frameNumber == attacks["basicF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicUpward(ushort frameNumber)
{
	if (ballRecover > 0) {
		return;
	}
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

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
	else if (frameNumber == attacks["basicU"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicU"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicDownward(ushort frameNumber)
{
	if (ballRecover > 0) {
		return;
	}
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

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
	else if (frameNumber == attacks["basicD"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicD"].hitBoxes[0]);
		CreateHitBox(&attacks["basicD"].hitBoxes[1]);
	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::SpecialNeutral(ushort frameNumber)
{
	if (ballRecover > 0) {
		return;
	}
	if (!onGround)
	{
		AllowMovement(0.3f);
	}

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
		superArmor = true;
	}
	else if (frameNumber == attacks["specialN"].keyFrames[0])
	{
		CreateHitBox(&attacks["specialN"].hitBoxes[0]);
	}
	else if (frameNumber >= attacks["specialN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
		superArmor = false;
	}
	
}

void Makt::SpecialForward(ushort frameNumber)
{
	if (ballRecover > 0) {
		return;
	}
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

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
		superArmor = true;
	}
	
	else if (frameNumber >= attacks["specialL"].keyFrames[0])
	{
		if (frameNumber == attacks["specialL"].keyFrames[0]) {
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
				timeHeld = frameNumber + 10;
				superArmor = false;
				ChangeMove([this](int f) { ThrowRecover(f); });
			}
			
		}	
		else if (input->down()) {
			currentMove = nullptr;
			moveFrame = 0;
			ballRecover = maxRecover;
			superArmor = false;
		}
	}
}

void Makt::ThrowRecover(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == attacks["specialLThrow"].keyFrames[0])
	{
		CreateHitBox(&attacks["specialLThrow"].hitBoxes[0]);
		ThrowBall(attacks["specialLThrow"].hitBoxes[0].hitdata, timeHeld);
	}
	else if (frameNumber >= attacks["specialLThrow"].totalFrames)
	{
		currentMove = nullptr;
	}
}


void Makt::ThrowBall(HitData force, ushort timeHeld)
{
	if (timeHeld > 200)
	{
		timeHeld = 200;
	}

	jumpStr = baseJump;
	maxSpeed = baseSpeed - ralentizar;
	weight = baseWeight;

	HitData aux = force;

	aux.damage += timeHeld / 10;
	aux.base += timeHeld / 10;

	ball = new MaktBall(manager, b2Vec2(body->GetPosition().x + dir * 4, body->GetPosition().y + 0.3f), aux, b2Vec2(dir, 0), respawnPos, playerNumber);
	manager->AddEntity(ball, layer, false);

	animAddon = "";
}

void Makt::SpecialUpward(ushort frameNumber)
{
	if (ballRecover > 0) {
		return;
	}
	if (!onGround)
	{
		AllowMovement(0.8f);
	}

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
	else if (frameNumber == attacks["specialU"].keyFrames[0])
	{
		anim->StartAnimation("especialU");

		CreateHitBox(&attacks["specialU"].hitBoxes[0]);
		
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -70));
	}
	else if (frameNumber >= attacks["specialU"].totalFrames)
	{
		recovery = false;
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::SpecialDownward(ushort frameNumber)
{
	if (ballRecover > 0) {
		return;
	}

	if (!onGround)
	{
		AllowMovement(0.2f);
	}

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
		superArmor = true;
	}
	else if (frameNumber == attacks["specialD"].keyFrames[0])
	{
		CreateHitBox(&attacks["specialD"].hitBoxes[0]);
	}
	else if (frameNumber >= attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
		superArmor = false;
	}
}

void Makt::BallPickUp(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		//sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["pickBall"].keyFrames[0])
	{
		if (SDL_HasIntersection(&hurtbox, ball->GetHurtbox()))
		{
			anim->StartAnimation("ballPick");
			RecoveredBall();
		}
	}
	else if (frameNumber >= attacks["pickBall"].totalFrames)
	{
		currentMove = nullptr;
	}
}

void Makt::RecoveredBall()
{
	if (ball->PickUp())
	{
		jumpStr = ballJump;
		maxSpeed = ballSpeed - ralentizar;
		weight = ballWeight;
		animAddon = "B";
		ball = nullptr;
	}
}

void Makt::update() {
	if (ballRecover > 0) {
		ballRecover--;
	}
	Character::update();
}

bool Makt::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) {
	if (superArmor) {
		superArmor = false;
		damageTaken += a.damage;
		return true;
	}
	return Character::GetHit(a, attacker, controlHitLag, controlShake, controlCamShake);
}

void Makt::BuildBoxes()
{
	attacks["basicN"].hitBoxes[0].box = 
		manager->GetSDLCoors(
		body->GetPosition().x + (dir * .6f),
		body->GetPosition().y,
		width * 1.8f,
		height);

	attacks["basicF"].hitBoxes[0].box = 
		manager->GetSDLCoors(
		body->GetPosition().x + (dir * 1.2f),
		body->GetPosition().y,
		width * 1.4f,
		height);
	attacks["basicF"].hitBoxes[1].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.2f),
			body->GetPosition().y,
			width * 1.6f,
			height * 1.2f);

	attacks["basicU"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir),
		body->GetPosition().y - height * .6f,
		width * 2.f,
		height * 1.5f);

	attacks["basicD"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir * width * 0.7f),
		body->GetPosition().y + height / 2,
		width * 2,
		height / 2);
	attacks["basicD"].hitBoxes[1].box = manager->GetSDLCoors(
		body->GetPosition().x - (dir * width * 0.7f),
		body->GetPosition().y + height / 2,
		width * 2,
		height / 2);

	attacks["specialN"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir * 1.7f),
		body->GetPosition().y,
		width * 1.7f,
		height);

	attacks["specialLThrow"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir * 1.4f),
		body->GetPosition().y,
		width * 1.3f,
		height);

	attacks["specialU"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir * 1.3f),
		body->GetPosition().y - height * 0.7f,
		width * 1.5f,
		height * 1.3f);

	attacks["specialD"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y + height * 0.2f,
		width * 3,
		height * 0.7f);


}
