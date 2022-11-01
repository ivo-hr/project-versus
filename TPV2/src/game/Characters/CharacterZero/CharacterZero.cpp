#include "CharacterZero.h"
#include "../../Utils/AnimationManager.h"
#include "../../../utils/CheckML.h"

CharacterZero::CharacterZero(FightManager* mngr, b2Vec2 pos, char input, ushort p) : Character(mngr, pos, input,p)
{

	//guardamos la textura
	texture = &sdl->images().at("zero");
	portrait = &sdl->images().at("zeroSelect");

	//importamos json del personaje
	spriteSheetData spData;
	ReadJson("resources/config/Characters/zero.json", spData);
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

	eyePos = { (float)hurtbox.w / 1.1f, (float)hurtbox.h / 6.f };

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
	else if (frameNumber == attacks["fuerte"].keyFrames[0] + 1)
	{
		ushort numHit = 0;
		for (pair<Entity*, bool> hit : isHit)
		{
			if (hit.second)
			{
				numHit++;
				if (hit.first->GetLives() == 1)
					numHit++;
			}
		}

		if (rand() % 150 < numHit)
		{
			Music::haltMusic();
			SoundEffect::haltChannel();

			char a[] = { "Player   won" };
			a[7] = playerNumber + 49;
			SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING, "Gg well played", a, sdl->window());

			manager->userExit();
		}
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
