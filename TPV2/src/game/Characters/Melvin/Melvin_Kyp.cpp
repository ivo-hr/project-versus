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

Melvin_Kyp::Melvin_Kyp(FightManager* mngr, b2Vec2 pos, char input, InputConfig* input_, ushort p):
	Character(mngr, pos, input, p, 2.3f, 2.5f)
{
	delete this->input;
	this->input = input_;

	//guardamos la textura
	texture = &sdl->images().at("kyp");
	portrait = &sdl->images().at("kypSelect");

	spriteSheetData spData;

	ReadJson("resources/config/Characters/melvin_kyp.json", spData);

	eyePos = { (float)0, (float)0 };

	anim = new AnimationManager(this, texture, spData);
}

Melvin_Kyp::~Melvin_Kyp()
{
	if (melvin)
	{
		melvin->HandledDelete();
		delete melvin;
		davin->HandledDelete();
		delete davin;
		cientifico->HandledDelete();
		delete cientifico;
	}
}

void Melvin_Kyp::BasicNeutral(ushort frameNumber)
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
void Melvin_Kyp::BasicForward(ushort frameNumber)
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
void Melvin_Kyp::BasicUpward(ushort frameNumber)
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
	else if (frameNumber == attacks["basicU"].keyFrames[1])
	{
		CreateHitBox(&attacks["basicU"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicU"].keyFrames[2])
	{
		CreateHitBox(&attacks["basicU"].hitBoxes[1]);
	}
	else if (frameNumber == attacks["basicU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin_Kyp::BasicDownward(ushort frameNumber)
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
		CreateHitBox(&attacks["basicD"].hitBoxes[1]);
		CreateHitBox(&attacks["basicD"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

//--------------------------------------------------------------------------------------------------

void Melvin_Kyp::SpecialNeutral(ushort frameNumber)
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

void Melvin_Kyp::SpecialForward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == attacks["specialF"].totalFrames)
	{
		Melvin::TransformInto(this, davin);
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin_Kyp::SpecialUpward(ushort frameNumber)
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

void Melvin_Kyp::SpecialDownward(ushort frameNumber)
{
	if (!onGround)
	{
		AllowMovement(0.1f);
	}

	if (frameNumber == 0)
	{
	}
	if (frameNumber == attacks["specialD"].keyFrames[0])
	{
	}
	if (frameNumber == attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin_Kyp::BuildBoxes()
{
	attacks["basicN"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y,
			width * 1.4f,
			height * 1.4f);

	attacks["basicF"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir),
			body->GetPosition().y,
			width * 1.5f,
			height * 1.8f);

	attacks["basicD"].hitBoxes[1].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * width * 0.9f),
			body->GetPosition().y,
			width * 1.8f,
			height / 2);

	attacks["basicD"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * width * 1.8f),
			body->GetPosition().y,
			width * 0.6f,
			height / 2);

	attacks["basicU"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y - height * 0.6f,
			width * 1.2f,
			height * 1.5f);

	attacks["basicU"].hitBoxes[1].box =
		manager->GetSDLCoors(
			body->GetPosition().x,
			body->GetPosition().y - height * 0.6f,
			width * 1.2f,
			height * 1.5f);

	/*
	attacks["basicU"].hitBoxes[0].specialEffect = 
		[this](Entity* f, Entity* a)
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
