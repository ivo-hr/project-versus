#include "Makt.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "Ball.h"
#include <fstream>
#include <iostream>

Makt::Makt(FightManager* mngr, b2Vec2 pos, char input) :
	Character(mngr, pos, input, 2.f, 3.5f)
{

	json js = ReadJson("resources/config/maketo.json");

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
}

Makt::~Makt()
{
	if (ball != nullptr)
	{
		manager->RemoveEntity(ball);
	}
}

void Makt::BasicNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN");
	}
	else if (frameNumber == attacks["basicN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * .6f),
			body->GetPosition().y,
			width * 1.8f,
			height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicN"], 3, OnHitData(12, false, false)));
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicForward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicF");
		sdl->soundEffects().at("catAtk1").play();

		body->SetLinearVelocity(b2Vec2(dir * 30, body->GetLinearVelocity().y));
	}
	else if (frameNumber == attacks["basicF"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir),
			body->GetPosition().y,
			width * 1.4f,
			height);

		attackData aaa = attacks["basicF"];

		if (!onGround)
		{
			aaa.direction.y = -10;
			aaa.direction.Normalize();
		}

		hitboxes.push_back(new Hitbox(hitbox, aaa, 5, OnHitData(20, false, false)));
	}
	else if (frameNumber == attacks["basicF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
	}
	else if (frameNumber == attacks["basicU"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * .2f),
			body->GetPosition().y - height * .5f,
			width * 1.8f,
			height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 5, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::BasicDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("basicD");
	}
	else if (frameNumber == attacks["basicD"].startUp)
	{
		SDL_Rect sweetspot = manager->GetSDLCoors(
				body->GetPosition().x + (dir * width),
				body->GetPosition().y + height * .4f,
				width,
				height * 0.5f);

		hitboxes.push_back(new Hitbox(sweetspot, attacks["basicDS"], 3, OnHitData(16, false, false)));

		SDL_Rect sourspot = manager->GetSDLCoors(
			body->GetPosition().x - (dir * width * 0.5f),
			body->GetPosition().y + height * .4f,
			width * 2,
			height * 0.5f);

		hitboxes.push_back(new Hitbox(sourspot, attacks["basicD"], 3, OnHitData(10, false, false)));

	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::SpecialNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (ball != nullptr)
		{
			ChangeMove([this](int f) { BallPickUp(f); });
			return;
		}
		moving = false;
		anim->StartAnimation("especialN");
		//sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["specialN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x + (dir * 1.2f),
			body->GetPosition().y,
			width * 1.7f,
			height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialN"], 8, OnHitData(30, false, false)));
	}
	else if (frameNumber >= attacks["specialN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
	
}

void Makt::SpecialForward(int frameNumber)
{

	if (frameNumber == 0)
	{
		if (ball != nullptr)
		{
			ChangeMove([this](int f) { BallPickUp(f); });
		}
		moving = false;
		anim->StartAnimation("especialL");
		//sdl->soundEffects().at("catAtk3").play();
	}
	else if (!input->special() && frameNumber >= attacks["specialL"].startUp)
	{
		ThrowBall(attacks["specialL"], frameNumber);
		ChangeMove([this](int f) {ThrowRecover(f); });
	}
}

void Makt::SpecialUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (ball != nullptr)
		{
			ChangeMove([this](int f) { BallPickUp(f); });
		}
		moving = false;
		anim->StartAnimation("especialU");
		//sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["specialU"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body,
			width * 1.2f,
			height * 1.2f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialU"], 20, Vector2D(-5 + (5*dir), -10), OnHitData(3, false, false)));

		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -60));
	}
	else if (frameNumber >= attacks["specialU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::SpecialDownward(int frameNumber)
{

	if (frameNumber == 0)
	{
		if (ball != nullptr)
		{
			ChangeMove([this](int f) { BallPickUp(f); });
		}
		moving = false;
		anim->StartAnimation("especialD");
		//sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["specialD"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y + height * 0.2f,
			width * 3,
			height * 0.7f);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialD"], 4, OnHitData(3, false, false)));
	}
	else if (frameNumber >= attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Makt::ThrowRecover(int frameNumber)
{
	if (frameNumber >= attacks["specialL"].totalFrames)
	{
		currentMove = nullptr;
	}
}

void Makt::BallPickUp(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("ballPick");
		//sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["pickBall"].startUp)
	{
		if (SDL_HasIntersection(&hurtbox, ball->GetHurtbox()))
			RecoveredBall();
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
		maxSpeed = ballSpeed;
		weight = ballWeight;
		ball = nullptr;
	}
}

void Makt::ThrowBall(attackData force, int timeHeld)
{
	if (timeHeld > 200)
	{
		timeHeld = 200;
	}

	jumpStr = baseJump;
	maxSpeed = baseSpeed;
	weight = baseWeight;

	attackData aux = force;

	aux.damage += timeHeld / 6;
	aux.base += timeHeld / 6;

	ball = new MaktBall(manager, b2Vec2( body->GetPosition().x + dir, body->GetPosition().y ), aux, b2Vec2(dir, 0), respawnPos);
	manager->AddEntity(ball);
	ball->SetOponents(oponents);
}
