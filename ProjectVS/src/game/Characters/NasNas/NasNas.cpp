#include "NasNas.h"
#include "Spell.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "../../Utils/Particle.h"
#include "../../../utils/CheckML.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

NasNas::NasNas(FightManager* mngr, b2Vec2 pos, char input, ushort p) : Character(mngr, pos, input,p, 1.5f, 2.7f)
{
	//guardamos la textura
	texture = &sdl->images().at("nasnasFire");
	portrait = &sdl->images().at("nasNasSelect");

	spriteSheetData spData;

	ReadJson("resources/config/Characters/nasnas.json", spData);

	eyePos = { (float)hurtbox.w / 1.6f, (float)hurtbox.h / 4.4f };

	anim = new AnimationManager(this, texture, spData);
}

NasNas::~NasNas()
{

}

//Lo mismo que el de arriba pero mas rapido y debil xd
void NasNas::BasicNeutral(ushort frameNumber)
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
void NasNas::BasicForward(ushort frameNumber)
{

	AllowMovement(0.5f, false, true);

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
void NasNas::BasicUpward(ushort frameNumber)
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

void NasNas::BasicDownward(ushort frameNumber)
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

void NasNas::SpecialNeutral(ushort frameNumber)
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
		anim->StartAnimation("especialN");
	}
	else if (frameNumber == attacks["specialN"].keyFrames[0])
	{
		mana -= 150;
		Spell* spell = nullptr;
		if (estado == fire)
		{
			attacks["specialN"].hitBoxes[0].hitdata.estado = fire;
			attacks["specialN"].hitBoxes[0].hitdata.power = 25;
			sdl->soundEffects().at("nasSpecNf").play();

			spell = new Spell(manager, b2Vec2(body->GetPosition().x + dir * 3, body->GetPosition().y - 0.3f), attacks["specialN"].hitBoxes[0].hitdata, b2Vec2(dir, 0), estado);
		}
		else if (estado == water)
		{
			attacks["specialN"].hitBoxes[1].hitdata.estado = water;
			attacks["specialN"].hitBoxes[1].hitdata.power = 20;
			sdl->soundEffects().at("nasSpecNw").play();

			spell = new Spell(manager, b2Vec2(body->GetPosition().x + dir * 3, body->GetPosition().y - 0.3f), attacks["specialN"].hitBoxes[1].hitdata, b2Vec2(dir, 0), estado);
		}
		else if (estado == electric)
		{
			attacks["specialN"].hitBoxes[2].hitdata.estado = electric;
			attacks["specialN"].hitBoxes[2].hitdata.power = 25;
			sdl->soundEffects().at("nasSpecNr").play(); 

			spell = new Spell(manager, b2Vec2(body->GetPosition().x + dir * 3, body->GetPosition().y - 0.3f), attacks["specialN"].hitBoxes[2].hitdata, b2Vec2(dir, 0), estado);
		}
		manager->AddEntity(spell, layer);
		manager->MoveToFront(spell);
	}
	else if (frameNumber == attacks["specialN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void NasNas::SpecialForward(ushort frameNumber)
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
		mana -= 300;
	}
	else if (frameNumber == attacks["specialF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void NasNas::SpecialUpward(ushort frameNumber)
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
		anim->StartAnimation("especialUFall");
		body->SetLinearVelocity(b2Vec2(0, 70));
	}
	else if (frameNumber > attacks["specialU"].keyFrames[1] && onGround)
	{
		mana -= 450;
		ChangeMove([this](int f) { SpecialUpHit(f); });
	}
	else if (frameNumber >= attacks["specialU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void NasNas::SpecialDownward(ushort frameNumber)
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
			texture = &sdl->images().at("nasnasWater");
		}
		else if (estado == water)
		{
			estado = electric;
			texture = &sdl->images().at("nasnasElectric");
		}
		else if (estado == electric)
		{
			estado = fire;
			texture = &sdl->images().at("nasnasFire");
		}
	}
	if (frameNumber == attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}

}

void NasNas::SpecialUpHit(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.9f);
	}

	if (frameNumber == 0)
	{
		manager->SetShake(Vector2D(-dir, 8), 10);
		anim->StartAnimation("especialUHit");
		sdl->soundEffects().at("nasSpecU").play();
	}
	else if (frameNumber == attacks["specialUHit"].keyFrames[0])
	{
		if (estado == fire)
		{
			CreateHitBox(&attacks["specialUHit"].hitBoxes[0]);
		}
		else if (estado == water)
		{
			CreateHitBox(&attacks["specialUHit"].hitBoxes[1]);
		}
		else if (estado == electric)
		{
			CreateHitBox(&attacks["specialUHit"].hitBoxes[2]);
		}
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
		mana += manaRecoverRate;
	}
}

void NasNas::drawHUD(ushort numOfPlayer)
{
	Character::drawHUD(numOfPlayer);

	int w_ = manager->GetActualWidth();
	int h_ = manager->GetActualHeight();
	int dist = w_ / numOfPlayer;
	int offset = (w_ / 2) / numOfPlayer - w_ / 30;
	int x = (int)(playerPosition * dist + offset) + (w_ / 14);
	int y = (h_ - (h_ / 6)) + w_ / 46;

	SDL_Rect aaa = { x, y, w_ / 15, w_ / 45 };

	SDL_SetRenderDrawColor(sdl->renderer(), 0x53, 0x1d, 0x1e, 0x5f);
	SDL_RenderFillRect(sdl->renderer(), &aaa);

	SDL_Rect aaab = { x, y, (int)(((float)w_ / 15.f) * ((float)mana / (float)maxMana)), w_ / 45 };

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
		width * 1.2f,
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
		width * 3.5f,
		height / 6);

	attacks["specialF"].hitBoxes[2].hitdata.estado = electric;
	attacks["specialF"].hitBoxes[2].hitdata.power = 50;


	attacks["specialUHit"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y,
		width * 3,
		height * 1.5f);

	attacks["specialUHit"].hitBoxes[1].box = manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y,
		width * 3,
		height * 1.5f);

	attacks["specialUHit"].hitBoxes[2].box = manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y,
		width * 3,
		height * 1.5f);

	attacks["specialUHit"].hitBoxes[0].hitdata.estado = fire;
	attacks["specialUHit"].hitBoxes[0].hitdata.power = 70;
	attacks["specialUHit"].hitBoxes[1].hitdata.estado = water;
	attacks["specialUHit"].hitBoxes[1].hitdata.power = 70;
	attacks["specialUHit"].hitBoxes[2].hitdata.estado = electric;
	attacks["specialUHit"].hitBoxes[2].hitdata.power = 70;
}

