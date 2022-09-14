#include "Yuno.h"
#include "../../Utils/AnimationManager.h"
#include "../GatoEspia/bullet.h"
#include "../../../json/json.hpp"
#include <fstream>
#include "../../../utils/CheckML.h"
#include <iostream>

using json = nlohmann::json;
Yuno::Yuno(FightManager* mngr, b2Vec2 pos, char input, ushort p) : Character(mngr, pos, input, p, 1.5f, 3.f)
{
	spriteSheetData spData;

	ReadJson("resources/config/bubble.json", spData);
	//guardamos la textura
	texture = &sdl->images().at("blinkMaster");
	portrait = &sdl->images().at("blinkMasterSelect");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

	anim = new AnimationManager(this, texture, spData);

	blinkContainer = &sdl->images().at("blinkCont");
	blinkfondo = &sdl->images().at("blinkContb");
}

Yuno::~Yuno()
{
	
}

//Lo mismo que el de arriba pero mas rapido y debil xd
void Yuno::BasicNeutral(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialNL");
		sdl->soundEffects().at("catSpecN").play();

		body->SetLinearVelocity(b2Vec2(dir * 40, body->GetLinearVelocity().y));
	}
	else if (frameNumber == attacks["basicN"].keyFrames[0])
	{
		if (boosted) {
			auto bullet = new Bullet(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - width / 2), attacks["basicN"].hitBoxes[1].hitdata, b2Vec2(dir, 0), 0.8f, 0.4f, 30);
			manager->AddEntity(bullet);
			bullet->SetOponents(oponents);
			boosted = false;
		}
		else {
			auto bullet = new Bullet(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - width / 2), attacks["basicN"].hitBoxes[0].hitdata, b2Vec2(dir, 0), 0.8f, 0.4f, 30);
			manager->AddEntity(bullet);
			bullet->SetOponents(oponents);
		}
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Yuno::BasicForward(ushort frameNumber)
{

	/*if (!onGround)
	{
		AllowMovement(0.7f);
	}*/

	if (frameNumber == 0)
	{
		anim->StartAnimation("basicF");
		sdl->soundEffects().at("catAtk0").play();
		speed = dir * 40;
	}
	else if (frameNumber == attacks["basicF"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicF"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicF"].keyFrames[1])
	{
		speed = dir * 40;
	}
	else if (frameNumber == attacks["basicF"].keyFrames[2])
	{
		CreateHitBox(&attacks["basicF"].hitBoxes[1]);
	}
	else if (frameNumber == attacks["basicF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
	
}

void Yuno::BasicDownward(ushort frameNumber)
{

	if (frameNumber == 0)
	{
		anim->StartAnimation("basicD");
		sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["basicD"].keyFrames[0])
	{
		if (boosted == false) {
			boosted = true;
		}
	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Yuno::BasicUpward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		sdl->soundEffects().at("catAtk2").play();

		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -10));
	}
	else if (frameNumber == attacks["basicU"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicU"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicU"].keyFrames[1])
	{
		if (onGround)
			CreateHitBox(&attacks["basicU"].hitBoxes[1]);
		else
			CreateHitBox(&attacks["basicU"].hitBoxes[2]);
	}
	else if (frameNumber == attacks["basicU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

//--------------------------------------------------------------------------------------------------

void Yuno::SpecialNeutral(ushort frameNumber)
{

	
}

void Yuno::SpecialNeutralU(ushort frameNumber)
{

	
}

void Yuno::SpecialNeutralD(ushort frameNumber)
{

	
}

void Yuno::SpecialForward(ushort frameNumber)
{

	
}

void Yuno::SpecialUpward(ushort frameNumber)
{

}

void Yuno::SpecialDownward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.3f);
	}

	if (frameNumber == 0)
	{
		if (blinks < 1.0f) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		anim->StartAnimation("especialD");
		sdl->soundEffects().at("catSpecD").play();
		blinks -= 1.0f;
	}
	else if (frameNumber == attacks["specialD"].keyFrames[0])
	{
		if (counter == false) {
			counter = true;
		}
		else {
			CreateHitBox(&attacks["specialD"].hitBoxes[0]);
			counter = false;
		}
	}
	else if (frameNumber == attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
	//else if (frameNumber == attacks["specialD"].totalFrames + 30)
	//{
	//	anim->StartAnimation("especialDSalida");
	//	currentMove = nullptr;
	//	moveFrame = -1;
	//}
}


void Yuno::update()
{
	Character::update();
	if (blinks < maxBlinks) {
		blinks += blinkRecover;
	}
}

void Yuno::Respawn()
{
	Character::Respawn();
	blinks = maxBlinks;
}

bool Yuno::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake)
{
	if (counter) {
		anim->StartAnimation("counter");
		anim->update();
		a.damage = 0;
		counter = false;
	}
	return Character::GetHit(a, attacker, controlHitLag, controlShake, controlCamShake);
}


void Yuno::drawHUD(ushort numOfPlayer)
{
	Character::drawHUD(numOfPlayer);

	int w_ = manager->GetActualWidth();
	int h_ = manager->GetActualHeight();
	int dist = w_ / numOfPlayer;
	int offset = (w_ / 2) / numOfPlayer - w_ / 30;
	int x = (int)(playerPosition * dist + offset) - (w_ / 30);
	int y = h_ - (h_ / 6) + w_ / 100;

	//portrait->render({ x, y, w_ / 15, w_ / 15 });

	SDL_Rect cont = {
		x,
		y,
		(w_ / 50),
		w_ / 17 };

	blinkfondo->render(cont);

	SDL_Rect blinkFill = {
		x + w_ / 300,
		y + (w_ / 17) - ((w_ / 17) * blinks / maxBlinks),
		(w_ / 70),
		w_ / 17 * blinks / maxBlinks };

	SDL_SetRenderDrawColor(sdl->renderer(), 0x53, 0xed, 0xee, 0xff);
	SDL_RenderFillRect(sdl->renderer(), &blinkFill);

	blinkContainer->render(cont);
}

void Yuno::BuildBoxes()
{

	attacks["basicF"].hitBoxes[0].box =
		manager->GetSDLCoors(
		body->GetPosition().x + (dir),
		body->GetPosition().y,
		width * 2.f,
		height * 0.8f);

	attacks["basicF"].hitBoxes[1].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir),
			body->GetPosition().y,
			width * 2.f,
			height * 0.8f);

	attacks["basicU"].hitBoxes[0].box = 
		manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y - height * 0.6f,
		width*1.5,
		height * 0.5f);
	attacks["basicU"].hitBoxes[1].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.2f),
			body->GetPosition().y,
			width * 1.2f,
			height);
	attacks["basicU"].hitBoxes[2].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.2f),
			body->GetPosition().y,
			width *1.2f,
			height);

	attacks["specialU"].hitBoxes[0].box = 
		manager->GetSDLCoors(body, width, height);

	attacks["specialLHit"].hitBoxes[0].box = 
		manager->GetSDLCoors(body, width * 1.8f, height * 0.6f);

	attacks["specialD"].hitBoxes[0].box =
		manager->GetSDLCoors(body->GetPosition().x,
			body->GetPosition().y-1.5,
			width * 3,
			height * 1.5f);
}
