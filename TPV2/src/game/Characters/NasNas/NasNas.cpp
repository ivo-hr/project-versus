#include "NasNas.h"
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

	spriteSheetData spData;

	ReadJson("resources/config/nasnas.json", spData);
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

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN");
		sdl->soundEffects().at("nasAtk0").play();
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
void NasNas::BasicForward(int frameNumber)
{

	AllowMovement(0.4f, false, true);

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicFWalk");
		sdl->soundEffects().at("nasAtk1").play();
	}
	else if (frameNumber == attacks["basicF"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicF"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}
void NasNas::BasicUpward(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		sdl->soundEffects().at("nasAtk2").play();
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

void NasNas::BasicDownward(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicD");
		sdl->soundEffects().at("nasAtk3").play();
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

//--------------------------------------------------------------------------------------------------

void NasNas::SpecialNeutral(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

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
	else if (frameNumber == attacks["specialN"].keyFrames[0])
	{
		Spell* spell = nullptr;
		if (estado == fire)
		{
			attacks["specialN"].hitBoxes[0].hitdata.estado = fire;
			attacks["specialN"].hitBoxes[0].hitdata.power = 25;
			sdl->soundEffects().at("nasSpecNf").play();

			spell = new Spell(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y), attacks["specialN"].hitBoxes[0].hitdata, b2Vec2(dir, 0), estado);
		}
		else if (estado == water)
		{
			attacks["specialN"].hitBoxes[1].hitdata.estado = water;
			attacks["specialN"].hitBoxes[1].hitdata.power = 20;
			sdl->soundEffects().at("nasSpecNw").play();

			spell = new Spell(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y), attacks["specialN"].hitBoxes[1].hitdata, b2Vec2(dir, 0), estado);
		}
		else if (estado == electric)
		{
			attacks["specialN"].hitBoxes[2].hitdata.estado = electric;
			attacks["specialN"].hitBoxes[2].hitdata.power = 25;
			sdl->soundEffects().at("nasSpecNr").play(); 

			spell = new Spell(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y), attacks["specialN"].hitBoxes[2].hitdata, b2Vec2(dir, 0), estado);
		}
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

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

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
	else if (frameNumber == attacks["specialF"].keyFrames[0])
	{
		if (estado == fire)
		{
			sdl->soundEffects().at("nasSpecSf").play();

			CreateHitBox(&attacks["specialF"].hitBoxes[0]);
		}
		else if (estado == water)
		{
			sdl->soundEffects().at("nasSpecSw").play();

			CreateHitBox(&attacks["specialF"].hitBoxes[1]);
		}
		else if (estado == electric)
		{
			sdl->soundEffects().at("nasSpecSr").play();

			CreateHitBox(&attacks["specialF"].hitBoxes[2]);
		}

	
	}
	else if (frameNumber == attacks["specialF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void NasNas::SpecialUpward(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.5f);
	}

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
	else if (frameNumber == attacks["specialU"].keyFrames[0])
	{
		body->SetLinearVelocity(b2Vec2(0, -70));
		sdl->soundEffects().at("nasSpecUup").play();
	}
	else if (frameNumber == attacks["specialU"].keyFrames[1])
	{
		body->SetLinearVelocity(b2Vec2(0, 70));
	}
	else if (frameNumber > attacks["specialU"].keyFrames[1] && onGround)
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

	if (!onGround)
	{
		AllowMovement(0.1f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialD");
		sdl->soundEffects().at("nasSpecD").play();
	}
	if (frameNumber == attacks["specialD"].keyFrames[0])
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
	}
	if (frameNumber == attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}

}

void NasNas::SpecialUpHit(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.9f);
	}

	if (frameNumber == 0)
	{
		anim->StartAnimation("especialUHit");

		sdl->soundEffects().at("nasSpecU").play();
	}
	else if (frameNumber == attacks["specialUHit"].keyFrames[0])
	{
		if (estado == fire)
		{
			attacks["specialUHit"].hitBoxes[0].hitdata.damage = 35;
			attacks["specialUHit"].hitBoxes[0].hitdata.base = 25;
			attacks["specialUHit"].hitBoxes[0].hitdata.estado = fire;
		}
		else if (estado == water)
		{
			attacks["specialUHit"].hitBoxes[0].hitdata.damage = 30;
			attacks["specialUHit"].hitBoxes[0].hitdata.base = 35;
			attacks["specialUHit"].hitBoxes[0].hitdata.estado = water;
		}
		else if (estado == electric)
		{
			attacks["specialUHit"].hitBoxes[0].hitdata.damage = 20;
			attacks["specialUHit"].hitBoxes[0].hitdata.base = 0.1;
			attacks["specialUHit"].hitBoxes[0].hitdata.estado = electric;
		}
		CreateHitBox(&attacks["specialUHit"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["specialUHit"].totalFrames)
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

void NasNas::BuildBoxes()
{
	attacks["basicN"].hitBoxes[0].box =
		manager->GetSDLCoors(
		body->GetPosition().x + dir * width,
		body->GetPosition().y,
		width * 2.5f,
		height * 0.25f);

	attacks["basicF"].hitBoxes[0].box = 
		manager->GetSDLCoors(
		body->GetPosition().x + dir * width,
		body->GetPosition().y,
		width * 1.8f,
		height * 1.02f);

	attacks["basicU"].hitBoxes[0].box = 
		manager->GetSDLCoors(
		body->GetPosition().x + dir * 0.2f,
		body->GetPosition().y - height * 0.9f,
		width * 2.4f,
		height * 0.9f);

	attacks["basicD"].hitBoxes[0].box = 
		manager->GetSDLCoors(
		body->GetPosition().x + (dir * width * 0.7f),
		body->GetPosition().y + height / 2,
		width * 1.4f,
		height / 2);
	attacks["basicD"].hitBoxes[1].box = manager->GetSDLCoors(
		body->GetPosition().x - (dir * width * 0.7f),
		body->GetPosition().y + height / 2,
		width * 1.4f,
		height / 2);

	attacks["specialF"].hitBoxes[0].box = //fire 
		manager->GetSDLCoors(
		body->GetPosition().x + (dir * width * 2),
		body->GetPosition().y - height * 0.2f,
		width,
		height * 1.3f);

	attacks["specialF"].hitBoxes[0].hitdata.estado = fire;
	attacks["specialF"].hitBoxes[0].hitdata.power = 50;

	attacks["specialF"].hitBoxes[1].box = //water
		manager->GetSDLCoors(
		body->GetPosition().x + (dir * width * 2.5f),
		body->GetPosition().y,
		width * 2,
		height / 3);

	attacks["specialF"].hitBoxes[1].hitdata.estado = water;
	attacks["specialF"].hitBoxes[1].hitdata.power = 45;

	attacks["specialF"].hitBoxes[2].box = //electric
		manager->GetSDLCoors(
		body->GetPosition().x + (dir * width * 3),
		body->GetPosition().y,
		width * 3.5,
		height / 6);

	attacks["specialF"].hitBoxes[2].hitdata.estado = electric;
	attacks["specialF"].hitBoxes[2].hitdata.power = 50;


	attacks["specialUHit"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y,
		width * 3,
		height * 1.5f);

	attacks["specialUHit"].hitBoxes[0].hitdata.power = 70;
}

