#include "Yuno.h"
#include "../../Utils/AnimationManager.h"
#include "../GatoEspia/bullet.h"
#include "../../../json/json.hpp"
#include <fstream>
#include "../../../utils/CheckML.h"
#include "YunoBubble.h"
#include <iostream>
using json = nlohmann::json;

Yuno::Yuno(FightManager* mngr, b2Vec2 pos, char input, ushort p) : Character(mngr, pos, input, p, 1.5f, 3.f)
{
	//guardamos la textura
	texture = &sdl->images().at("blinkMaster");
	portrait = &sdl->images().at("yunoSelect");

	spriteSheetData spData;

	ReadJson("resources/config/Characters/bubble.json", spData);
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

	anim = new AnimationManager(this, texture, spData);
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
			auto bullet = new Bullet(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - width / 2), attacks["basicN"].hitBoxes[1].hitdata, b2Vec2(dir, 0), 1.f, 0.3f, 30, "bulletYuno");
			manager->AddEntity(bullet, layer);
			boosted = false;
		}
		else {
			auto bullet = new Bullet(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - width / 2), attacks["basicN"].hitBoxes[0].hitdata, b2Vec2(dir, 0), 1.f, 0.3f, 30, "bulletYuno");
			manager->AddEntity(bullet, layer);
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
		speed = dir * 40.f;
	}
	else if (frameNumber == attacks["basicF"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicF"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicF"].keyFrames[1])
	{
		speed = dir * 40.f;
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
	if (frameNumber == 4)
	{
		releasedSpec = false;
		if (!bubble && !explotado)
		{
			bubble = new YunoBubble(manager, body->GetPosition(), this, 3, 2, Bubble::NEUTRAL, input);
			manager->AddEntity(bubble, 0);
		}
	}

	if (!input->special())
	{
		releasedSpec = true;
	}

	if (input->special() && bubble && releasedSpec)
	{
		bubble->setToDelete();
		explotado = true;	
	}

	if (frameNumber > 4 && !bubble)
	{
		currentMove = nullptr;
	}
	
}

void Yuno::SpecialForward(ushort frameNumber)
{
	if (frameNumber == 4)
	{
		if (!bubble)
		{
			bubble = new YunoBubble(manager, body->GetPosition()+b2Vec2(dir, 0), this, 1, 2, Bubble::FORWARD);
			manager->AddEntity(bubble, 0);
		}
	}
	if (frameNumber == attacks["specialL"].totalFrames) {
		currentMove = nullptr;
	}
}

void Yuno::SpecialUpward(ushort frameNumber)
{
	if (frameNumber == 4)
	{
		if (!bubble && !explotado)
		{
			bubbled = true;
			bubble = new YunoBubble(manager, body->GetPosition(), this, 5, 0, Bubble::UP, input);
			manager->AddEntity(bubble, 0);
		}
	}
	if (frameNumber > 4 && !bubble)
	{
		currentMove = nullptr;
		recovery = false;
	}
}

void Yuno::SpecialDownward(ushort frameNumber)
{
	
	if (!onGround)
		{
			AllowMovement(0.3f);
		}

		if (frameNumber == 0)
		{
			anim->StartAnimation("especialD");
			sdl->soundEffects().at("catSpecD").play();
		}
		else if (frameNumber == attacks["specialD"].keyFrames[0])
		{
			if (!casco) {
				casco = true;
			}
			else {
				CreateHitBox(&attacks["specialD"].hitBoxes[0]);
				casco = false;
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

bool Yuno::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake)
{
	if (bubble)
	{
		bubble->setToDelete();
	}
	if (casco)
	{
		casco = false;
		a.damage = 0;
	}
	return Character::GetHit(a, attacker, controlHitLag, controlShake, controlCamShake);
}

void Yuno::BubblePopped()
{
	bubble = nullptr;
	bubbled = false;
}

void Yuno::update()
{
	if (explotado && lastBubble < 10) {
		lastBubble++;
	}
	else {
		explotado = false;
		lastBubble = 0;
	}
	Character::update();
}

void Yuno::OnDeath()
{
	if (bubble)
	{
		bubble->setToDelete();
		bubble = nullptr;
	}
	Character::OnDeath();
}

bool Yuno::IsOutOfBounds()
{
	return Character::IsOutOfBounds() && !bubbled;
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
		width * 1.5f,
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

	attacks["specialD"].hitBoxes[0].box =
		manager->GetSDLCoors(body->GetPosition().x,
			body->GetPosition().y - 1.5f,
			width,
			height * 0.5f);
}
