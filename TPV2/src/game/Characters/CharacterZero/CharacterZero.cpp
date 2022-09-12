#include "CharacterZero.h"
#include "../../Utils/AnimationManager.h"
#include "../../../utils/CheckML.h"

CharacterZero::CharacterZero(FightManager* mngr, b2Vec2 pos, char input, ushort p) : Character(mngr, pos, input,p)
{
	//importamos json del personaje

	spriteSheetData spData;
	
	ReadJson("resources/config/zero.json", spData);

	//guardamos la textura
	texture = &sdl->images().at("zero");
	portrait = &sdl->images().at("zeroSelect");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

	anim = new AnimationManager(this, texture, spData);
}

CharacterZero::~CharacterZero()
{

}

void CharacterZero::draw()
{
	Character::draw();
}


void CharacterZero::BasicNeutral(ushort frameNumber)
{
	if (!onGround)
	{
		AllowMovement(0.7f, true, false);
	}
	else
	{
		AllowMovement(1, true, false);
	}

	if (frameNumber == 0)
	{
		anim->StartAnimation("big");
		sdl->soundEffects().at("zeroSpecN").play();
	}
	else if (frameNumber == attacks["fuerte"].keyFrames[0])
	{
		CreateHitBox(&attacks["fuerte"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["fuerte"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void CharacterZero::BasicForward(ushort frameNumber)
{
	ChangeMove([this](int f) { BasicNeutral(f); });
}

void CharacterZero::BasicUpward(ushort frameNumber)
{
	ChangeMove([this](int f) { BasicNeutral(f); });
}

void CharacterZero::BasicDownward(ushort frameNumber)
{
	ChangeMove([this](int f) { BasicNeutral(f); });
}



//Lo mismo que el de arriba pero mas lento y fuerte xd
void CharacterZero::SpecialNeutral(ushort frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}
	else
	{
		AllowMovement();
	}

	if (frameNumber == 0)
	{
		anim->StartAnimation("small");
	}
	else if (frameNumber == attacks["debil"].keyFrames[0])
	{
		CreateHitBox(&attacks["debil"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["debil"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void CharacterZero::SpecialForward(ushort frameNumber)
{
	ChangeMove([this](int f) { SpecialNeutral(f); });
}

void CharacterZero::SpecialUpward(ushort frameNumber)
{
	ChangeMove([this](int f) { SpecialNeutral(f); });
}

void CharacterZero::SpecialDownward(ushort frameNumber)
{
	ChangeMove([this](int f) { SpecialNeutral(f); });
}

void CharacterZero::BuildBoxes()
{
	attacks["debil"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * .8f),
			body->GetPosition().y,
			width * 0.8f,
			height);
	
	attacks["fuerte"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + width,
			body->GetPosition().y,
			width * 0.5f,
			height * 0.5f);

}
