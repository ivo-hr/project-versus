#include "Melvin.h"
#include "Melvin_Davin.h"
#include "Melvin_Kyp.h"
#include "Melvin_Cientifico.h"
#include "../../Utils/AnimationManager.h"
#include "../../Utils/MyListener.h"
#include "../../../json/json.hpp"
#include "../../Utils/Particle.h"
#include "../../../utils/CheckML.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

Melvin_Davin::Melvin_Davin(FightManager* mngr, b2Vec2 pos, char input, InputConfig* input_, ushort p) :
	Character(mngr, pos, input, p, 2.4f, 3.f)
{
	delete this->input;
	this->input = input_;

	//guardamos la textura
	texture = &sdl->images().at("davin");
	portrait = &sdl->images().at("davinSelect");

	spriteSheetData spData;

	ReadJson("resources/config/Characters/melvin_davin.json", spData);

	eyePos = { (float)0, (float)0 };

	anim = new AnimationManager(this, texture, spData);
}

Melvin_Davin::~Melvin_Davin()
{
	if (melvin)
	{
		melvin->HandledDelete();
		delete melvin;
		kyp->HandledDelete();
		delete kyp;
		cientifico->HandledDelete();
		delete cientifico;
	}
}

void Melvin_Davin::BasicNeutral(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
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
void Melvin_Davin::BasicForward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
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
void Melvin_Davin::BasicUpward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
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

void Melvin_Davin::BasicDownward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
	}
	else if (frameNumber == attacks["basicD"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicD"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

//--------------------------------------------------------------------------------------------------

void Melvin_Davin::SpecialNeutral(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == attacks["specialN"].totalFrames)
	{
		Melvin::TransformInto(this, melvin);
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin_Davin::SpecialForward(ushort frameNumber)
{
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
	}
	else if (frameNumber == attacks["specialF"].keyFrames[0])
	{
		CreateHitBox(&attacks["specialF"].hitBoxes[0]);
	}
	else if (frameNumber > attacks["specialF"].keyFrames[0] && frameNumber < attacks["specialF"].totalFrames)
	{
		speed = dir * 30.f;
		body->SetLinearVelocity({ body->GetLinearVelocity().x, -1.f });
	}
	else if (frameNumber > attacks["specialF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin_Davin::SpecialUpward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == attacks["specialU"].totalFrames)
	{
		Melvin::TransformInto(this, cientifico);
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin_Davin::SpecialDownward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == attacks["specialD"].totalFrames)
	{
		Melvin::TransformInto(this, kyp);
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin_Davin::BuildBoxes()
{
	attacks["basicN"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir),
			body->GetPosition().y,
			width * 1.f,
			height * 1.f);

	attacks["basicF"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir),
			body->GetPosition().y,
			width * 1.2f,
			height * 1.f);

	attacks["basicD"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * width * 0.6f),
			body->GetPosition().y + height / 2,
			width * 1.f,
			height / 2);

	attacks["basicU"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y - height * 0.6f,
			width * 1.2f,
			height * 1.5f);

	attacks["specialF"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir),
			body->GetPosition().y,
			width * 0.5f,
			height * 0.8f);

	/*
	attacks["basicU"].hitBoxes[0].specialEffect = 
		[this](Entity* a)
		{
			auto w = static_cast<Character*>(a);
			if (w)
			{
				w->ResetChar();
			}
			a->AddHitLag(1000);
			attacks["basicU"].hitBoxes[0].hitdata.isValid = false;
		};
	*/
}
