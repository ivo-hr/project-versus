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

Melvin_Cientifico::Melvin_Cientifico(FightManager* mngr, b2Vec2 pos, char input, InputConfig* input_, ushort p) :
	Character(mngr, pos, input, p, 1.f, 3.f)
{
	delete this->input;
	this->input = input_;
	ogInput = this->input;

	//guardamos la textura
	texture = &sdl->images().at("cientifico");
	portrait = &sdl->images().at("cientificoSelect");

	spriteSheetData spData;

	ReadJson("resources/config/Characters/melvin_cientifico.json", spData);

	eyePos = { (float)0, (float)0 };

	anim = new AnimationManager(this, texture, spData);
}

Melvin_Cientifico::~Melvin_Cientifico()
{
	if (melvin)
	{
		melvin->HandledDelete();
		delete melvin;
		kyp->HandledDelete();
		delete kyp;
		davin->HandledDelete();
		delete davin;
	}
}

void Melvin_Cientifico::BasicNeutral(ushort frameNumber)
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
void Melvin_Cientifico::BasicForward(ushort frameNumber)
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
void Melvin_Cientifico::BasicUpward(ushort frameNumber)
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

void Melvin_Cientifico::BasicDownward(ushort frameNumber)
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

void Melvin_Cientifico::SpecialNeutral(ushort frameNumber)
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

void Melvin_Cientifico::SpecialForward(ushort frameNumber)
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

void Melvin_Cientifico::SpecialUpward(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.5f);
	}

	if (frameNumber == 0)
	{
	}
	else if (frameNumber >= attacks["specialU"].keyFrames[0] && frameNumber < attacks["specialU"].totalFrames)
	{
		body->SetLinearVelocity({ body->GetLinearVelocity().x, -30.f });
		if (!input->special())
		{
			recovery = !recovery;
			currentMove = nullptr;
			moveFrame = -1;
		}
	}
	else if (frameNumber >= attacks["specialU"].totalFrames)
	{
		recovery = !recovery;
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin_Cientifico::SpecialDownward(ushort frameNumber)
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

void Melvin_Cientifico::BuildBoxes()
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

	attacks["basicD"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * width * 0.6f),
			body->GetPosition().y + height / 2,
			width * 1.f,
			height / 2);

	attacks["basicD"].hitBoxes[1].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * width * 1.2f),
			body->GetPosition().y + height / 2,
			width * 0.5f,
			height / 2);

	attacks["basicU"].hitBoxes[0].box =
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