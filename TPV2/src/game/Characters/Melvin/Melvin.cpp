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

Melvin::Melvin(FightManager* mngr, b2Vec2 pos, char input, ushort p) : Character(mngr, pos, input, p, 2.0f, 1.7f)
{
	//guardamos la textura
	texture = &sdl->images().at("melvin");
	portrait = &sdl->images().at("melvinSelect");

	spriteSheetData spData;

	ReadJson("resources/config/Characters/melvin.json", spData);

	eyePos = { (float)0, (float)0 };

	anim = new AnimationManager(this, texture, spData);

	davin = new Melvin_Davin(manager, pos, input, this->input, p);
	kyp = new Melvin_Kyp(manager, pos, input, this->input, p);
	cientifico = new Melvin_Cientifico(manager, pos, input, this->input, p);

	davin->SetOtherChar(this, kyp, cientifico);
	kyp->SetOtherChar(this, davin, cientifico);
	cientifico->SetOtherChar(this, davin, kyp);
}

Melvin::~Melvin()
{
	if (possesedChar)
	{
		input = nullptr;
		delete possesedInput;
	}
	if (kyp)
	{
		kyp->HandledDelete();
		delete kyp;
		davin->HandledDelete();
		delete davin;
		cientifico->HandledDelete();
		delete cientifico;
	}
}

void Melvin::update()
{
	if (toPosses && hitLag <= 1)
	{
		possesedChar = toPosses->GetInputConfig()->OriginalOwner();
		possesedLayer = possesedChar->GetLayer();
		possesedInput = possesedChar->GetInputConfig();
		possesedChar->SetInputConfig(input);
		SetPosition(toPosses->GetBody()->GetPosition());
		alive = false;
		manager->ChangeEntityLayer(possesedChar, layer);
		//body->SetEnabled(false);
		toPosses = nullptr;
	}
	if (possesedChar)
	{
		if (possesedChar->GetInputConfig()->OriginalOwner() != input->OriginalOwner())
		{
			possesedChar->SetInputConfig(input);
			manager->ChangeEntityLayer(possesedChar, layer);
		}
		possesTimer++;
		if (possesTimer >= possesionTime)
			UnPosses();
		return;
	}
	Character::update();
}

void Melvin::BasicNeutral(ushort frameNumber)
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
void Melvin::BasicForward(ushort frameNumber)
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
void Melvin::BasicUpward(ushort frameNumber)
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

void Melvin::BasicDownward(ushort frameNumber)
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
	else if (frameNumber == attacks["basicD"].keyFrames[1])
	{
		CreateHitBox(&attacks["basicD"].hitBoxes[1]);
	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

//--------------------------------------------------------------------------------------------------

void Melvin::SpecialNeutral(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
	}
	else if (frameNumber == attacks["specialN"].keyFrames[0])
	{
		readyToPosses = true;
	}
	else if (frameNumber == attacks["specialN"].keyFrames[1])
	{
		readyToPosses = false;
	}
	else if (frameNumber == attacks["specialN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin::SpecialForward(ushort frameNumber)
{
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == attacks["specialF"].totalFrames)
	{
		TransformInto(this, davin);
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin::SpecialUpward(ushort frameNumber)
{
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == attacks["specialU"].totalFrames)
	{
		TransformInto(this, cientifico);
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Melvin::SpecialDownward(ushort frameNumber)
{
	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == attacks["specialD"].totalFrames)
	{
		TransformInto(this, kyp);
		currentMove = nullptr;
		moveFrame = -1;
	}
}

bool Melvin::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake)
{
	if (readyToPosses && attacker->HasTag(Tags::IsCharacter))
	{
		Posses(attacker, controlHitLag, controlCamShake);
		return false;
	}
	readyToPosses = false;
	return Character::GetHit(a, attacker, controlHitLag, controlShake, controlCamShake);
}

void Melvin::OnEntityAdded()
{
}

void Melvin::Posses(Entity* attacker, bool& controlHitLag, bool& controlCamShake)
{
	toPosses = static_cast<Character*>(attacker);
	AddHitLag(20);
	attacker->AddHitLag(20);
	manager->MoveToFront(this);
	controlHitLag = controlCamShake = true;

	float xEyeDiff = eyePos.getX() - (hurtbox.w / 2.f);
	AddParticle("parryS", { hurtbox.x + (hurtbox.w / 2.f) - (xEyeDiff * dir), hurtbox.y + eyePos.getY() }, dir, true);
	sdl->soundEffects().at("parry").play();
	readyToPosses = false;
}

void Melvin::UnPosses()
{
	body->SetEnabled(true);
	SetPosition(possesedChar->GetBody()->GetPosition());
	ResetChar();

	possesedChar->SetInputConfig(possesedInput);
	manager->ChangeEntityLayer(possesedChar, possesedLayer);
	possesedChar->ResetChar();

	possesedChar = nullptr;
	possesedInput = nullptr;
	possesedLayer = 0;
	alive = true;
	possesTimer = 0;
}

void Melvin::TransformInto(Character* current, Character* to)
{
	current->GetManager()->GetEntityReferenceTo(current) = to;
	current->GetManager()->GetCharacterReferenceTo(current) = to;
	current->GetManager()->GetCameraReferenceTo(current) = to;
	current->GetManager()->GetMatrixReferenceTo(current) = to;

	current->GetManager()->GetListener()->AddCharacter(to);
	current->GetManager()->GetListener()->RemoveCharacter(current);
	to->SetGround(current->GetGround());
	to->SetLayer(current->GetLayer());
	to->SetPlaceInLayer(current->GetPlaceInLayer());
	to->SetPNumber(current->GetPNumber());
	to->SetPosition(current->GetBody()->GetPosition());
	to->SetDamage(current->GetDamageTaken());
	to->GetBody()->ApplyLinearImpulseToCenter({ 0.1f, 0 }, true);

	current->GetOgInput()->SetOriginalOwner(to);
}

void Melvin::BuildBoxes()
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

