#include "Togo.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "../../Utils/Particle.h"
#include "Spear.h"
#include "DinoShield.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

Togo::Togo(FightManager* mngr, b2Vec2 pos, char input,int p) : Character(mngr, pos, input,p, 1.5f, 3.5f)
{

	ReadJson("resources/config/dino.json");
	//guardamos la textura
	texture = &sdl->images().at("dinoSouls");
	portrait = &sdl->images().at("dinoSoulsSelect");

	anim = new AnimationManager(this, texture, spData);
}

Togo::~Togo()
{

}

//Lo mismo que el de arriba pero mas rapido y debil xd
void Togo::BasicNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN");
		sdl->soundEffects().at("dinoAtk0").play();
	}
	else if (frameNumber == attacks["basicN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * 2.f),
			body->GetPosition().y,
			width * 4.f,
			height * 0.4f);
		SDL_Rect sweetspot = manager->GetSDLCoors(
			body->GetPosition().x + (dir * 5.5f),
			body->GetPosition().y,
			width * 1.1f,
			height * 0.5f);

		/*hitbox.h /= 6;
		hitbox.w *= 4;
		hitbox.y += hitbox.h / 2;
		hitbox.y += (hitbox.h + 10);
		if (dir == -1)
		{
			hitbox.x -= hitbox.w-20;
		}
		else 
		{
			hitbox.x += 15;

		}*/

		attackData a = attacks["basicN"];
		a.damage *= 1.2f;
		a.base *= 1.2f;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicN"], 2, OnHitData(5, false, false)));
		hitboxes.push_back(new Hitbox(sweetspot, a, 2, OnHitData(8, false, false)));
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::BasicForward(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (!lanza) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		anim->StartAnimation("basicF");
		sdl->soundEffects().at("dinoAtk1").play();
	}
	else if (frameNumber == attacks["basicF"].startUp)
	{
			auto spear = new Spear(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y-height/2), attacks["basicF"], b2Vec2(dir, 0), this);
			manager->AddEntity(spear);
			manager->MoveToFront(spear);
			spear->SetOponents(oponents);
			SetSpear(false);
	}
	else if (frameNumber == attacks["basicF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::BasicUpward(int frameNumber)
{
	attackData a = attacks["basicU"];

	a.base = 0;
	a.multiplier = 0;
	 
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		sdl->soundEffects().at("dinoAtk2").play();
	}
	else if (frameNumber == attacks["basicU"].startUp)
	{
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, body->GetLinearVelocity().y / 5 });
		if (!onGround) {
			body->SetGravityScale(0.0f);
		}

		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y - height * 0.7f,
			width * 4.f,
			height * 0.4f);

		hitboxes.push_back(new Hitbox(hitbox, a, 3, OnHitData(3, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+3)
	{

		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y - height * 0.7f,
			width * 4.f,
			height * 0.4f);

		hitboxes.push_back(new Hitbox(hitbox, a, 3, OnHitData(3, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+6)
	{

		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y - height * 0.7f,
			width * 4.f,
			height * 0.4f);

		hitboxes.push_back(new Hitbox(hitbox, a, 3, OnHitData(3, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+9)
	{

		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y - height * 0.7f,
			width * 4.f,
			height * 0.4f);

		hitboxes.push_back(new Hitbox(hitbox, a, 3, OnHitData(3, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+12)
	{

		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y - height * 0.7f,
			width * 4.f,
			height * 0.4f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 3, OnHitData(15, false, false)));
		body->SetGravityScale(10.0f);
	}
	else if (frameNumber == attacks["basicU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::BasicDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicD");
		sdl->soundEffects().at("dinoAtk3").play();
	}
	else if (frameNumber == attacks["basicD"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.8f),
			body->GetPosition().y + height * 0.3f,
			width * 2.6f,
			height * 0.5f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicD"], 4, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicD"].startUp+attacks["basicD"].totalFrames/4)
	{
		dir = -dir;

		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.8f),
			body->GetPosition().y + height * 0.3f,
			width * 2.6f,
			height * 0.5f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicD"], 4, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

//--------------------------------------------------------------------------------------------------

void Togo::SpecialNeutral(int frameNumber)
{
	if (onGround)
	{
		if (frameNumber == 0)
		{
			anim->StartAnimation("especialN");
			sdl->soundEffects().at("dinoSpecN").play();
			moving = false;
		}
		else if (frameNumber == attacks["specialN"].startUp)
		{
			anim->StartAnimation("especialNHold");
			dShield = new DinoShield(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - GetHeight() / 4.5));
			dShield->SetOponents(oponents);

			manager->AddEntity(dShield);

			manager->AddOponnent(dShield, this);

			manager->MoveToFront(dShield);

		}
		if (/*frameNumber == attacks["specialN"].totalFrames || */ !input->special())
		{
			anim->StartAnimation("idle");
			manager->RemoveEntity(dShield);
			dShield = nullptr;
			currentMove = nullptr;
			moveFrame = -1;
		}
	}
	else
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::SpecialForward(int frameNumber)
{
	if (frameNumber <= 0) {
		anim->StartAnimation("especialLEntrada");
		sdl->soundEffects().at("dinoSpecS").play();
	}
	else if (frameNumber < attacks["specialL"].totalFrames)
	{

		
		moving = false;
		body->SetLinearVelocity(b2Vec2(dir*30, 0));
		body->ApplyLinearImpulseToCenter(b2Vec2(dir*30,0), true);

		if (frameNumber >= attacks["specialL"].startUp && frameNumber < attacks["specialL"].totalFrames / 2 + 5) {
			bite = manager->GetSDLCoors(
				body->GetPosition().x + (dir * width * 1.8f),
				body->GetPosition().y - height * 0.2f,
				width * 4,
				height * 0.7f);

			for (int i = 0; i < oponents.size(); i++) {
				if (SDL_HasIntersection(&bite, oponents[i]->GetHurtbox())) {
					ChangeMove([this](int f) { SpecialLHit(f); });
				}
			}

#ifdef _DEBUG

			SDL_RenderDrawRect(sdl->renderer(), &bite);

#endif // _DEBUG

		}
		else if (frameNumber == attacks["specialL"].totalFrames / 2 + 5)
		{
			anim->StartAnimation("especialLSalida");
		}
	}
	else if (frameNumber == attacks["specialL"].totalFrames)
	{
		bite = { 0, 0, 0, 0 };
		recovery = false;
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::SpecialUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialU");
		sdl->soundEffects().at("dinoSpecU").play();
	}
	else if (frameNumber < attacks["specialU"].totalFrames) {
		if (frameNumber == attacks["specialU"].startUp)
		{
			SDL_Rect hitbox = manager->GetSDLCoors(
				body->GetPosition().x,
				body->GetPosition().y,
				width * 2,
				height * 1.2f);

			hitboxes.push_back(new Hitbox(hitbox, attacks["specialU"], 20, Vector2D(-hitbox.w / 4, 0), OnHitData(6, false, false)));

			body->SetGravityScale(0.0f);
			body->SetLinearVelocity(b2Vec2(0, -35));

		}
		if (body->GetLinearVelocity().x > 0 || body->GetLinearVelocity().x < 0) {
			body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
		}
		
	}
	else if (frameNumber == attacks["specialU"].totalFrames)
	{
		body->SetGravityScale(10.0f);
		currentMove = nullptr;
		moveFrame = -1;
		recovery = false;
		//body->SetGravityScale(10);

	}
}

void Togo::SpecialDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialD");
		sdl->soundEffects().at("dinoSpecD").play();
	}
	else if (frameNumber == attacks["specialD"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.9f),
			body->GetPosition().y + height * 0.3f,
			width * 2.8f,
			height * 0.7f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialD"], 4, OnHitData(18, false, false)));

	}
	else if (frameNumber == attacks["specialD"].startUp + attacks["specialD"].totalFrames/4)
	{
		dir = -dir;

		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.9f),
			body->GetPosition().y + height * 0.3f,
			width * 2.8f,
			height * 0.7f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialD"], 4, OnHitData(18, false, false)));
	}
	else if (frameNumber == attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::SetSpear(bool spear)
{
	lanza = spear;
}

void Togo::SpecialLHit(int frameNumber)
{
	if (frameNumber == 0) {
		anim->StartAnimation("especialLHit");
		moving = false;
		body->SetLinearVelocity(b2Vec2(0, 0));
	}
	else if (frameNumber == attacks["specialLHit"].startUp) {
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * width * 1.8f),
			body->GetPosition().y - height * 0.2f,
			width * 4.5f,
			height * 0.9f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialLHit"], 1, OnHitData(15, false, false)));
	}
	else if (frameNumber == attacks["specialLHit"].totalFrames) {
		currentMove = nullptr;
		moveFrame = -1;
	}
}

bool Togo::GetHit(attackData a, Entity* attacker)
{
	if (dShield != nullptr)
	{
		manager->RemoveEntity(dShield);
		dShield = nullptr;
	}
	Character::GetHit(a,attacker);
	return true;
}

void Togo::update()
{
	if (dShield != nullptr && stun > 0)
	{
		manager->RemoveEntity(dShield);
		dShield = nullptr;
	}
	Character::update();
}
