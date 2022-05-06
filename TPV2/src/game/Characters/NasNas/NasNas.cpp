﻿#include "NasNas.h"
#include "Spell.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "../../Utils/Particle.h"
#include "../../../utils/CheckML.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

NasNas::NasNas(FightManager* mngr, b2Vec2 pos, char input,int p) : Character(mngr, pos, input,p, 1.5f, 2.7f)
{

	ReadJson("resources/config/nasnas.json");
	//guardamos la textura
	texture = &sdl->images().at("nasnasFire");
	portrait = &sdl->images().at("nasNasSelect");

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
		sdl->soundEffects().at("nasAtk0").play();
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
		anim->StartAnimation("basicFWalk");
		sdl->soundEffects().at("nasAtk1").play();
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
	}
	if (input->right() && dir == 1)
	{
		if (currentMove == nullptr)
			dir = 1;

		if (speed < 1)
			AddParticle(new Particle(Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, "run", this));

		speed = maxSpeed-10;
	}
	if (input->left() && dir == -1)
	{
		if (currentMove == nullptr)
			dir = -1;

		if (speed > -1)
			AddParticle(new Particle(Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, "run", this));

		speed = -maxSpeed+10;
	}
}
void NasNas::BasicUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		sdl->soundEffects().at("nasAtk2").play();
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
		sdl->soundEffects().at("nasAtk3").play();
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
		if (mana < 150) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		else mana -= 150;
		anim->StartAnimation("especialN");
	}
	else if (frameNumber == attacks["specialN"].startUp)
	{
		attackData aaa = attacks["specialN"];
		if (estado == fire)
		{
			aaa.damage = 10;
			aaa.base = 10;
			aaa.estado = fire;
			aaa.power = 25;
			sdl->soundEffects().at("nasSpecNf").play();
		}
		else if (estado == water)
		{
			aaa.damage = 5;
			aaa.base = 15;
			aaa.estado = water;
			aaa.power = 20;
			sdl->soundEffects().at("nasSpecNw").play();
		}
		else if (estado == electric)
		{
			aaa.damage = 7;
			aaa.base = 7;
			aaa.estado = electric;
			aaa.power = 25;
			sdl->soundEffects().at("nasSpecNr").play();
		}
		auto spell = new Spell(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y), aaa, b2Vec2(dir, 0), estado);
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
	if (frameNumber == 0)
	{
		if (mana < 300) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		mana -= 300;
		moving = false;
		anim->StartAnimation("especialF");
		

	}
	else if (frameNumber == attacks["specialF"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);
		attackData aaa = attacks["specialF"];
		if (estado == fire)
		{
			hitbox.w *= 3;
			if (dir == -1) {
				hitbox.x += width*11;
			}
			aaa.damage = 25;
			aaa.base = 15;
			aaa.estado = fire;
			aaa.power = 50;
			sdl->soundEffects().at("nasSpecSf").play();
		}
		else if (estado == water)
		{
			hitbox.x += width;
			if (dir == -1) {
				hitbox.x += width * 10;
			}
			hitbox.y += (hitbox.h / 2)-7;
			hitbox.h /= 3;
			hitbox.w *= 4;
			aaa.damage = 20;
			aaa.base = 25;
			aaa.estado = water;
			aaa.power = 45;
			sdl->soundEffects().at("nasSpecSw").play();
		}
		else if (estado == electric)
		{
			hitbox.x += width;
			if (dir == -1) {
				hitbox.x += width * 10;
			}
			hitbox.y += (hitbox.h / 2)-4;
			hitbox.h /= 6;
			hitbox.w *= 5.4;
			aaa.damage = 12;
			aaa.base = 1;
			aaa.estado = electric;
			aaa.power = 50;
			sdl->soundEffects().at("nasSpecSr").play();
		}
		if (dir == -1)
		{
			hitbox.x -= hitbox.w;
		}
			hitboxes.push_back(new Hitbox(hitbox, aaa, 10, OnHitData(20, false, false)));

	
	}
	else if (frameNumber == attacks["specialF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void NasNas::SpecialUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (mana < 450) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		mana -= 450;
		moving = false;
		anim->StartAnimation("especialU");


	}
	else if (frameNumber == attacks["specialU"].startUp)
	{
		body->SetLinearVelocity(b2Vec2(0, -70));
		sdl->soundEffects().at("nasSpecUup").play();
	}
	else if (frameNumber == attacks["specialU"].totalFrames)
	{
		body->SetLinearVelocity(b2Vec2(0, 70));
		anim->StartAnimation("especialUFall");
	}
	else if (frameNumber >= attacks["specialU"].totalFrames && onGround)
	{
		ChangeMove([this](int f) { SpecialUpHit(f); });
	}
	if (body->GetLinearVelocity().x > 0 || body->GetLinearVelocity().x < 0)
	{
		body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
	}
}

void NasNas::SpecialDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialD");
		sdl->soundEffects().at("nasSpecD").play();
	}
	if (frameNumber == attacks["specialD"].startUp)
	{
		
	}
	if (frameNumber == attacks["specialD"].totalFrames)
	{
		if (estado == fire)
		{
			estado = water;
			anim->ChangeSheet(&sdl->images().at("nasnasWater"));
		}
		else if (estado == water)
		{
			estado = electric;
			anim->ChangeSheet(&sdl->images().at("nasnasElectric"));
		}
		else if (estado == electric)
		{
			estado = fire;
			anim->ChangeSheet(&sdl->images().at("nasnasFire"));
		}
		currentMove = nullptr;
		moveFrame = -1;
	}

}

void NasNas::SpecialUpHit(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("especialUHit");
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);
		attackData aaa = attacks["specialU"];
		hitbox.h /= 1.5;
		hitbox.w *= 2;
		hitbox.x -= (hitbox.w / 4);
		hitbox.y += 15;
		aaa.power = 70;
		sdl->soundEffects().at("nasSpecU").play();
		if (estado == fire)
		{
			aaa.damage = 35;
			aaa.base = 25;
			aaa.estado = fire;
		}
		else if (estado == water)
		{
			aaa.damage = 30;
			aaa.base = 35;
			aaa.estado = water;
		}
		else if (estado == electric)
		{
			aaa.damage = 20;
			aaa.base = 10;
			aaa.estado = electric;
		}
		hitboxes.push_back(new Hitbox(hitbox, aaa, 6, OnHitData(20, false, false)));
	}
	if (frameNumber >= attacks["specialF"].totalFrames - attacks["specialU"].startUp)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void NasNas::update()
{
	Character::update();
	if (mana < 0) {
		mana = 0;
	}
	if (mana < maxMana) {
		mana ++;
	}
}

void NasNas::drawHUD( int numOfPlayer)
{
	Character::drawHUD(numOfPlayer);

	int w_ = manager->GetDeathZone()->w;
	int h_ = manager->GetDeathZone()->h;
	int dist = w_ / numOfPlayer;
	int offset = (w_ / 2) / numOfPlayer - w_ / 30;
	int x = (int)(playerPosition * dist + offset) + (w_ / 14);
	int y = (h_ - (h_ / 6)) + w_ / 46;

	SDL_Rect aaa = { x, y, w_ / 15, w_ / 45 };

	SDL_SetRenderDrawColor(sdl->renderer(), 0x53, 0x1d, 0x1e, 0x5f);
	SDL_RenderFillRect(sdl->renderer(), &aaa);

	SDL_Rect aaab = { x, y, (w_ / 15) * ((float)mana / (float)maxMana), w_ / 45 };

	SDL_SetRenderDrawColor(sdl->renderer(), 0x53, 0xed, 0xee, 0xff);
	SDL_RenderFillRect(sdl->renderer(), &aaab);
}

void NasNas::Respawn()
{
	Character::Respawn();
	mana = maxMana;
}


