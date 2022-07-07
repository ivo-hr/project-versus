#include "GatoEspia.h"
#include "../../Utils/AnimationManager.h"
#include "bullet.h"
#include "../../../json/json.hpp"
#include <fstream>
#include "../../../utils/CheckML.h"
#include <iostream>
using json = nlohmann::json;
GatoEspia::GatoEspia(FightManager* mngr, b2Vec2 pos, char input,int p) : Character(mngr, pos, input,p, 1.5f, 3.f)
{

	spriteSheetData spData;

	ReadJson("resources/config/gato.json", spData);
	//guardamos la textura
	texture = &sdl->images().at("blinkMaster");
	portrait = &sdl->images().at("blinkMasterSelect");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

	anim = new AnimationManager(this, texture, spData);

	blinkContainer = &sdl->images().at("blinkCont");
	blinkfondo = &sdl->images().at("blinkContb");
}

GatoEspia::~GatoEspia()
{
	
}

//Lo mismo que el de arriba pero mas rapido y debil xd
void GatoEspia::BasicNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN");
		sdl->soundEffects().at("catAtk0").play();
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

void GatoEspia::BasicForward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicF");
		sdl->soundEffects().at("catAtk1").play();

		body->SetLinearVelocity(b2Vec2(dir * 40, body->GetLinearVelocity().y));
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

void GatoEspia::BasicDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicD");
		sdl->soundEffects().at("catAtk2").play();
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

void GatoEspia::BasicUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		sdl->soundEffects().at("catAtk3").play();
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

//--------------------------------------------------------------------------------------------------

void GatoEspia::SpecialNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialNL");
		sdl->soundEffects().at("catSpecN").play();
	}
	else if (frameNumber == attacks["specialN"].keyFrames[0])
	{
		auto bullet = new Bullet(manager, b2Vec2(body->GetPosition().x + dir * 2, body->GetPosition().y - height / 3), attacks["specialN"].hitBoxes[0].hitdata, b2Vec2(dir, 0));
		manager->AddEntity(bullet);
		bullet->SetOponents(oponents);
	}
	else if (frameNumber == attacks["specialN"].totalFrames)
	{
		//La cadena de ifs para cambiar a donde est� apuntando (hay una parecida en cada variante)
		if (!input->special())
		{
			currentMove = nullptr;
			moveFrame = -1;
		}
		else
		{
			if (input->up())
			{
				if (input->left())
				{
					dir = -1;
					currentMove = [this](int f) { SpecialNeutralD(f); };
				}
				else if (input->right())
				{
					dir = 1;
					currentMove = [this](int f) { SpecialNeutralD(f); };
				}
				else {
					currentMove = [this](int f) { SpecialNeutralU(f); };
					
				}
				moveFrame = -1;
			}
			else
			{
				if (input->left())
				{
					dir = -1;
				}
				else if (input->right())
				{
					dir = 1;
				}
				moveFrame = -1;
			}
		}
	}
}

void GatoEspia::SpecialNeutralU(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialNU");
		sdl->soundEffects().at("catSpecN").play();

	}
	else if (frameNumber == attacks["specialN"].keyFrames[0])
	{
		//SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		//hitbox.w *= 2.4f;
		//hitbox.h *= 0.7f;
		//hitbox.x -= hitbox.w / 3;
		//hitbox.y -= 45;

		//CreateHitBox(new Hitbox(hitbox, attacks["specialN"], 1, OnHitData(5, false, false)));
		auto bullet = new Bullet(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - width / 2), attacks["specialN"].hitBoxes[0].hitdata, b2Vec2(0,-1));
		manager->AddEntity(bullet);
		bullet->SetOponents(oponents);

	}
	else if (frameNumber == attacks["specialN"].totalFrames)
	{

		if (!input->special())
		{
			currentMove = nullptr;
			moveFrame = -1;
		}
		else
		{
			if (!input->up())
			{
				if (input->left())
				{
					dir = -1;
					currentMove = [this](int f) { SpecialNeutral(f); };
				}
				else if (input->right())
				{
					dir = 1;
					currentMove = [this](int f) { SpecialNeutral(f); };
				}
				moveFrame = -1;
			}
			else
			{
				if (input->left())
				{
					dir = -1;
					currentMove = [this](int f) { SpecialNeutralD(f); };
				}
				else if (input->right())
				{
					dir = 1;
					currentMove = [this](int f) { SpecialNeutralD(f); };
				}
				moveFrame = -1;
			}
		}
	}
}

void GatoEspia::SpecialNeutralD(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialND");
		sdl->soundEffects().at("catSpecN").play();
	}
	else if (frameNumber == attacks["specialN"].keyFrames[0])
	{
		auto bullet = new Bullet(manager, b2Vec2(body->GetPosition().x + dir / 2, body->GetPosition().y - height / 2), attacks["specialN"].hitBoxes[0].hitdata, b2Vec2(dir, -1));
		manager->AddEntity(bullet);
		bullet->SetOponents(oponents);
	}
	else if (frameNumber == attacks["specialN"].totalFrames)
	{

		if (!input->special())
		{
			currentMove = nullptr;
			moveFrame = -1;
		}
		else
		{
			if (!input->up())
			{
				if (input->left())
				{
					dir = -1;
					currentMove = [this](int f) { SpecialNeutral(f); };
				}
				else if (input->right())
				{
					dir = 1;
					currentMove = [this](int f) { SpecialNeutral(f); };
				}
				moveFrame = -1;
			}
			else
			{
				if (input->left())
				{
					dir = -1;
				}
				else if (input->right())
				{
					dir = 1;
				}
				else
				{
					currentMove = [this](int f) { SpecialNeutralU(f); };
				}
				moveFrame = -1;
			}
		}
	}
}

void GatoEspia::SpecialForward(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (blinks < 1.0f) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		body->SetLinearVelocity(b2Vec2(0, 0));
		body->SetGravityScale(0);
		anim->StartAnimation("especialL");
		sdl->soundEffects().at("catSpecS").play();
		moving = false;
		blinks -= 1.0f;
	}
	else if (frameNumber == attacks["specialL"].keyFrames[0])
	{
		dash = true;
	}
	else if (frameNumber == attacks["specialL"].keyFrames[1])
	{
		body->SetTransform(body->GetPosition() + b2Vec2(dir * 7, 0), 0);
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, 0 });
		body->SetGravityScale(10.0f);
		dash = false;

		if (input->special())
		{
			ChangeMove([this](int f) { TpAtack(f); });
			sdl->soundEffects().at("catAtk1").play();
		}
	}
	else if (frameNumber == attacks["specialL"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void GatoEspia::SpecialUpward(int frameNumber)
{

	if (frameNumber == 0)
	{
		if (blinks < 1.0f) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		anim->StartAnimation("especialU");
		sdl->soundEffects().at("catSpecU").play();
		body->SetLinearVelocity(b2Vec2(0, 0));
		body->SetGravityScale(0);
		moving = false;
		blinks -= 1.0f;
	}
	else if (frameNumber == attacks["specialU"].keyFrames[0])
	{
		dash = true;
	}
	else if (frameNumber == attacks["specialU"].keyFrames[1])
	{
		b2Vec2 a;
		if (input->left())
		{
			dir = -1;
			a = b2Vec2(-7, -7);
			a.Normalize();
			a *= 7;
		}
		else if (input->right())
		{
			dir = 1;
			a = b2Vec2(7, -7);
			a.Normalize();
			a *= 7;
		}
		else
		{
			a = b2Vec2(0, -7);
		}
		body->SetTransform(body->GetPosition() + a, 0);
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, -25 });
		body->SetGravityScale(10.0f);

		CreateHitBox(&attacks["specialU"].hitBoxes[0]);

	}
	else if (frameNumber == attacks["specialU"].keyFrames[2])
	{
		dash = false;
		body->SetLinearVelocity(b2Vec2(0, 0));
		body->SetGravityScale(0);
	}
	else if (frameNumber == attacks["specialU"].totalFrames)
	{
		body->SetGravityScale(10.0f);
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void GatoEspia::SpecialDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (blinks < 1.0f) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		anim->StartAnimation("especialD");
		sdl->soundEffects().at("catSpecD").play();
		blinks -= 1.0f;
	}
	else if (frameNumber == attacks["specialD"].keyFrames[0])
	{
		dash = true;	
		counter = true;
	}
	else if (frameNumber == attacks["specialD"].totalFrames) {
		dash = false;
		counter = false;
		currentMove = nullptr;
		moveFrame = -1;
	}
	//else if (frameNumber == attacks["specialD"].totalFrames + 30)
	//{
	//	anim->StartAnimation("especialDSalida");
	//	currentMove = nullptr;
	//	moveFrame = -1;
	//}
}

void GatoEspia::TpAtack(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("especialLHit");
	}
	else if (frameNumber == attacks["specialLHit"].keyFrames[0])
	{
		CreateHitBox(&attacks["specialLHit"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["specialLHit"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void GatoEspia::update()
{
	Character::update();
	if (blinks < maxBlinks) {
		blinks += blinkRecover;
	}
}

void GatoEspia::Respawn()
{
	Character::Respawn();
	blinks = maxBlinks;
	body->SetGravityScale(10.f);
}

bool GatoEspia::GetHit(HitData a, Entity* attacker)
{
	if (counter && !attacker->isProjectile()) {
		anim->StartAnimation("counter");
		anim->update();
		dir = attacker->GetDir();
		body->SetTransform(attacker->GetBody()->GetPosition() + b2Vec2(-dir * 2.4f, 0), 0);
		currentMove = [this](int f) { Counter(f); };
		moveFrame = -1;
		AddHitLag(20);
		attacker->AddHitLag(25);
		manager->MoveToFront(this);
		return false;
	}
	Character::GetHit(a, attacker);
}

void GatoEspia::Counter(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("especialDGolpe");
		body->SetLinearVelocity({ 0.01, 0.01 });
		dash = false;
		counter = false;
	}
	else if (frameNumber == attacks["specialDHit"].keyFrames[0])
	{
		CreateHitBox(&attacks["specialDHit"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["specialDHit"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}

}

void GatoEspia::drawHUD(int numOfPlayer)
{
	Character::drawHUD(numOfPlayer);

	int w_ = manager->GetDeathZone()->w;
	int h_ = manager->GetDeathZone()->h;
	int dist = w_ / numOfPlayer;
	int offset = (w_ / 2) / numOfPlayer - w_ / 30;
	int x = (int)(playerPosition * dist + offset) - (w_ / 30);
	int y = h_ - (h_ / 6) + w_ / 100;

	//portrait->render({ x, y, w_ / 15, w_ / 15 });

	SDL_Rect cont = {
		x,
		y,
		(w_ / 50),
		w_ / 17 };

	blinkfondo->render(cont);

	SDL_Rect blinkFill = {
		x + w_ / 300,
		y + (w_ / 17) - ((w_ / 17) * blinks / maxBlinks),
		(w_ / 70),
		w_ / 17 * blinks / maxBlinks };

	SDL_SetRenderDrawColor(sdl->renderer(), 0x53, 0xed, 0xee, 0xff);
	SDL_RenderFillRect(sdl->renderer(), &blinkFill);

	blinkContainer->render(cont);
}

void GatoEspia::BuildBoxes()
{
	attacks["basicN"].hitBoxes[0].box =
		manager->GetSDLCoors(
			body->GetPosition().x + (dir * .8f),
			body->GetPosition().y,
			width * 1.8f,
			height);

	attacks["basicF"].hitBoxes[0].box =
		manager->GetSDLCoors(
		body->GetPosition().x + (dir),
		body->GetPosition().y,
		width * 2.f,
		height * 0.8f);

	attacks["basicD"].hitBoxes[0].box = 
		manager->GetSDLCoors(
		body->GetPosition().x + (dir * width * 0.7f),
		body->GetPosition().y + height / 2,
		width * 1.4f,
		height / 2);

	attacks["basicD"].hitBoxes[1].box = 
		manager->GetSDLCoors(
		body->GetPosition().x - (dir * width * 0.7f),
		body->GetPosition().y + height / 2,
		width * 1.4f,
		height / 2);

	attacks["basicU"].hitBoxes[0].box = 
		manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y - height * 0.6f,
		width * 2.5f,
		height * 0.7f);

	attacks["specialU"].hitBoxes[0].box = 
		manager->GetSDLCoors(body, width, height);

	attacks["specialLHit"].hitBoxes[0].box = 
		manager->GetSDLCoors(body, width * 1.8f, height * 0.6f);

	attacks["specialDHit"].hitBoxes[0].box = 
		manager->GetSDLCoors(body, width * 2, height);
}
