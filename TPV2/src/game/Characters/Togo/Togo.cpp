#include "Togo.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "Spear.h"
#include "DinoShield.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

Togo::Togo(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input, 1.5f, 3.5f)
{

	ReadJson("resources/config/dino.json");
	//guardamos la textura
	texture = &sdl->images().at("dinoSouls");

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
		//sdl->soundEffects().at("catAtk0").play();
	}
	else if (frameNumber == attacks["basicN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

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

		hitbox.w *= 3;
		hitbox.y -= 30;
		if (dir == -1)
		{
			hitbox.x += -hitbox.w/1.5 -20;
		}
		else
		{
			hitbox.x += 20;

		}



		hitboxes.push_back(new Hitbox(hitbox, attacks["basicN"], 2, OnHitData(5, false, false)));
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
		//sdl->soundEffects().at("catAtk0").play();
	}
	else if (frameNumber == attacks["basicF"].startUp)
	{
			auto spear = new Spear(manager, new Vector2D(body->GetPosition().x, body->GetPosition().y-height/2), attacks["basicF"], b2Vec2(dir, 0), this);
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
	SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

	hitbox.w *= 2.4f;
	hitbox.h *= 0.7f;
	hitbox.x -= hitbox.w / 1.8;
	hitbox.y -= 20;
	hitbox.h = hitbox.h / 3;
	hitbox.w *= 1.5;
	 
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["basicU"].startUp)
	{
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, body->GetLinearVelocity().y / 5 });
		if (!onGround) {
			body->SetGravityScale(0.0f);
		}

		hitboxes.push_back(new Hitbox(hitbox, a, 3, OnHitData(3, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+3)
	{

		hitboxes.push_back(new Hitbox(hitbox, a, 3, OnHitData(3, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+6)
	{

		hitboxes.push_back(new Hitbox(hitbox, a, 3, OnHitData(3, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+9)
	{

		hitboxes.push_back(new Hitbox(hitbox, a, 3, OnHitData(3, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+12)
	{

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
		sdl->soundEffects().at("catAtk2").play();
	}
	else if (frameNumber == attacks["basicD"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);


		hitbox.h /= 2;
		hitbox.w *= 2.2;

		hitbox.y += GetHeight() * 10;

		if (dir == -1) {
			hitbox.x -= hitbox.w - GetWidth() * 19;
		}

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicD"], 4, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicD"].startUp+attacks["basicD"].totalFrames/4)
	{
		dir = -dir;
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);


		hitbox.h /= 2;
		hitbox.w *= 2.2;

		hitbox.y += GetHeight() * 10;

		if (dir == -1) {
			hitbox.x -= hitbox.w - GetWidth() * 19;
		}

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
			//sdl->soundEffects().at("catAtk1").play();
			moving = false;
		}
		else if (frameNumber == attacks["specialN"].startUp)
		{
			anim->StartAnimation("especialNHold");
			dShield = new DinoShield(manager, new Vector2D(body->GetPosition().x, body->GetPosition().y - GetHeight() / 4.5));
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
	if (frameNumber == 0) {
		anim->StartAnimation("especialLEntrada");
	}
	else if (frameNumber < attacks["specialL"].totalFrames)
	{

		//sdl->soundEffects().at("catAtk1").play();
		moving = false;
		body->SetLinearVelocity(b2Vec2(dir*30, 0));
		body->ApplyLinearImpulseToCenter(b2Vec2(dir*30,0), true);
		if (frameNumber >= attacks["specialL"].startUp && frameNumber < attacks["specialL"].totalFrames / 2 + 5) {
			bite = hurtbox;
			bite.w *= 3;
			if (dir == -1) {
				bite.x = hurtbox.x - bite.w / 1.5 - 20;
			}
			else {
				bite.x = hurtbox.x + 20;
			}
			bite.y = hurtbox.y -30;
			for (int i = 0; i < oponents.size(); i++) {
				if (SDL_HasIntersection(&bite, oponents[i]->GetHurtbox())) {
					currentMove = [this](int f) { SpecialLHit(f); };
					moveFrame = -1;
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
	}
	else if (frameNumber < attacks["specialU"].totalFrames) {
		if (frameNumber == attacks["specialU"].startUp)
		{
			SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);
			hitbox.w *= 2;
			hitbox.h += 10;
			hitboxes.push_back(new Hitbox(hitbox, attacks["specialU"], 20, Vector2D(-hitbox.w / 4, 0), OnHitData(6, false, false)));
			body->SetGravityScale(0.0f);
			body->SetLinearVelocity(b2Vec2(0, -25));

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
		//sdl->soundEffects().at("catAtk2").play();
	}
	else if (frameNumber == attacks["specialD"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.h /= 2;
		hitbox.w *= 3;

		hitbox.y += GetHeight()*10;

		if (dir == -1) {
			hitbox.x -= hitbox.w - GetWidth() * 19;
		}

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialD"], 4, OnHitData(18, false, false)));

	}
	else if (frameNumber == attacks["specialD"].startUp + attacks["specialD"].totalFrames/4)
	{
		dir = -dir;
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.h /= 2;
		hitbox.w *= 3;

		hitbox.y += GetHeight() * 10;

		if (dir == -1) {
			hitbox.x -= hitbox.w - GetWidth() * 19;
		}

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
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);
		hitbox.y -= 30;
		hitbox.w *= 3;
		hitbox.h = hitbox.h / 1.5;
		hitbox.h += 10;

		if (dir == -1)
		{
			hitbox.x -= hitbox.w - 20;
		}
		else
		{
			hitbox.x += 20;

		}

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialLHit"], 1, OnHitData(15, false, false)));
	}
	else if (frameNumber == attacks["specialLHit"].totalFrames) {
		currentMove = nullptr;
		moveFrame = -1;
	}
}
bool Togo::GetHit(attackData a, Entity* attacker)
{
	bite = SDL_Rect({ 0, 0, 0, 0 });

	if (shield)
	{
		damageTaken += (int)(a.damage * 0.4f);
		return true;
	}
	if (dash)
	{
		return false;
	}
	else if (!shield && !dash)
	{
		if (dShield != nullptr)
		{
			manager->RemoveEntity(dShield);
			dShield = nullptr;
		}
		currentMove = nullptr;
		moveFrame = -1;
		anim->StartAnimation("stun");
		anim->update();
		float recoil = (a.base + ((damageTaken * a.multiplier) / (weight * .2f)));

		stun = (recoil / 1.8f) + 4;

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


