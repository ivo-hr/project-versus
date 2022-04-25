#include "NasNas.h"
#include "Spell.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "../../Utils/Particle.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

NasNas::NasNas(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input, 1.5f, 3.5f)
{

	ReadJson("resources/config/nasnas.json");
	//guardamos la textura
	texture = &sdl->images().at("dinoSouls");

	anim = new AnimationManager(this, texture, spData);
}

NasNas::~NasNas()
{

}

//Lo mismo que el de arriba pero mas rapido y debil xd
void NasNas::BasicNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN");
		//sdl->soundEffects().at("catAtk0").play();
	}
	else if (frameNumber == attacks["basicN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.h /= 4;
		hitbox.w *= 2.5;
		hitbox.y += hitbox.h / 2;
		hitbox.y += (hitbox.h + 10);
		if (dir == -1)
		{
			hitbox.x -= hitbox.w-20;
		}
		else 
		{
			hitbox.x += 15;
		}

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicN"], 2, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}
void NasNas::BasicForward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicF");
		//sdl->soundEffects().at("catAtk1").play();

		maxSpeed -= 7;
	}
	else if (frameNumber == attacks["basicF"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.h += 20;
		hitbox.w *= 1.5;
		hitbox.x += dir * 50;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicF"], 1, Vector2D(dir * 50,hitbox.y), OnHitData(20, false, false)));
	}
	else if (frameNumber == attacks["basicF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
		maxSpeed += 7;
	}
	if (input->right())
	{
		if (currentMove == nullptr)
			dir = 1;

		if (speed < 1)
			AddParticle(new Particle(Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, "run", nullptr, this));

		speed = maxSpeed-7;
	}
	if (input->left())
	{
		if (currentMove == nullptr)
			dir = -1;

		if (speed > -1)
			AddParticle(new Particle(Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, "run", nullptr, this));

		speed = -maxSpeed+7;
	}
}
void NasNas::BasicUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		//sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["basicU"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 3;
		hitbox.y -= 45;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 10, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void NasNas::BasicDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicD");
		//sdl->soundEffects().at("catAtk2").play();
	}
	else if (frameNumber == attacks["basicD"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);


		hitbox.y += hitbox.h;
		hitbox.w *= 4;
		hitbox.h *= 0.5f;
		hitbox.x -= hitbox.w / 3;
		hitbox.y -= hitbox.h;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicD"], 1, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

//--------------------------------------------------------------------------------------------------

void NasNas::SpecialNeutral(int frameNumber)
{

	if (frameNumber == 0)
	{
		if (currentMana < 20) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		else currentMana -= 20;
		anim->StartAnimation("basicF");
		//sdl->soundEffects().at("catAtk0").play();
	}
	else if (frameNumber == attacks["specialN"].startUp)
	{
		auto spell = new Spell(manager, new Vector2D(body->GetPosition().x, body->GetPosition().y), attacks["specialN"], b2Vec2(dir, 0));
		manager->AddEntity(spell);
		manager->MoveToFront(spell);
		spell->SetOponents(oponents);
	}
	else if (frameNumber == attacks["specialN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void NasNas::SpecialForward(int frameNumber)
{
	currentMove = nullptr;
	moveFrame = -1;
}

void NasNas::SpecialUpward(int frameNumber)
{
	currentMove = nullptr;
	moveFrame = -1;
}

void NasNas::SpecialDownward(int frameNumber)
{
	currentMove = nullptr;
	moveFrame = -1;
}

bool NasNas::GetHit(attackData a, Entity* attacker)
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


