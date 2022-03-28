#include "Togo.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;
Togo::Togo(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input, 5.f, 5.5f)
{

	ReadJson("resources/config/dino.json");
	//guardamos la textura
	texture = &sdl->images().at("blinkMaster");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

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
		sdl->soundEffects().at("catAtk0").play();
	}
	else if (frameNumber == attacks["basicN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);
		
		hitbox.w = 220;
		hitbox.h = 30;
		hitbox.x += dir * 70;
		hitbox.y += hitbox.h+10;

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

}
void Togo::BasicUpward(int frameNumber)
{

}
void Togo::BasicDownward(int frameNumber)
{

}

//--------------------------------------------------------------------------------------------------

void Togo::SpecialNeutral(int frameNumber)
{

}

void Togo::SpecialForward(int frameNumber)
{

}

void Togo::SpecialUpward(int frameNumber)
{

}

void Togo::SpecialDownward(int frameNumber)
{

}


