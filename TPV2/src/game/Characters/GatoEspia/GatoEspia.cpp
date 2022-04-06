#include "GatoEspia.h"
#include "../../Utils/AnimationManager.h"
#include "bullet.h"
#include "../../../json/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;
GatoEspia::GatoEspia(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input, 1.5f, 3.f)
{

	ReadJson("resources/config/gato.json");
	//guardamos la textura
	texture = &sdl->images().at("blinkMaster");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

	anim = new AnimationManager(this, texture, spData);
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
	else if (frameNumber == attacks["basicN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 30;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicN"], 2, OnHitData(5, false, false)));
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

		body->SetLinearVelocity(b2Vec2(dir * 30, body->GetLinearVelocity().y));
	}
	else if (frameNumber == attacks["basicF"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 50;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicF"], 1, OnHitData(20, false, false)));
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
	else if (frameNumber == attacks["basicD"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);


		hitbox.y += hitbox.h;
		hitbox.w *= 2.5f;
		hitbox.h *= 0.3f;
		hitbox.x -= hitbox.w / 3;
		hitbox.y -= hitbox.h;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicD"], 1, OnHitData(5, false, false)));
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
	else if (frameNumber == attacks["basicU"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 3;
		hitbox.y -= 45;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 5, OnHitData(5, false, false)));
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
	else if (frameNumber == attacks["specialN"].startUp)
	{
		auto bullet = new Bullet(manager, new Vector2D(body->GetPosition().x, body->GetPosition().y),attacks["specialN"], b2Vec2(dir, 0));
		manager->AddEntity(bullet);			//QUE LA BALA SALGA DE LA PISTOLA Y EL MEMORY LEAK DE TEXTURE QUE DIJO SAMIR
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
	else if (frameNumber == attacks["specialN"].startUp)
	{
		//SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		//hitbox.w *= 2.4f;
		//hitbox.h *= 0.7f;
		//hitbox.x -= hitbox.w / 3;
		//hitbox.y -= 45;

		//hitboxes.push_back(new Hitbox(hitbox, attacks["specialN"], 1, OnHitData(5, false, false)));
		auto bullet = new Bullet(manager, new Vector2D(body->GetPosition().x, body->GetPosition().y), attacks["specialN"], b2Vec2(0,-1));
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
	else if (frameNumber == attacks["specialN"].startUp)
	{
		auto bullet = new Bullet(manager, new Vector2D(body->GetPosition().x, body->GetPosition().y), attacks["specialN"], b2Vec2(dir, -1));
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
		anim->StartAnimation("entrarTP");
		sdl->soundEffects().at("catSpecS").play();
		moving = false;
		blinks -= 1.0f;
	}
	else if (frameNumber == attacks["specialL"].startUp / 2)
	{
		dash = true;
	}
	else if (frameNumber == attacks["specialL"].startUp-1)
	{
		body->SetTransform(body->GetPosition() + b2Vec2(dir * 7, 0), 0);
		anim->StartAnimation("salirTP");
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, 0 });
		body->SetGravityScale(10.0f);
		dash = false;

		if (input->special())
		{
			currentMove = [this](int f) { TpAtack(f); };
			moveFrame = -5;
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
		anim->StartAnimation("entrarTP");
		sdl->soundEffects().at("catSpecU").play();
		body->SetLinearVelocity(b2Vec2(0, 0));
		body->SetGravityScale(0);
		moving = false;
		blinks -= 1.0f;
	}
	else if (frameNumber == attacks["specialU"].startUp / 2)
	{
		dash = true;
	}
	//No me pregunten por que pero tengo que poner esto para que se vea bienxd
	//else if (frameNumber == attacks["specialU"].startUp - 2)
	//{
	//	SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);
	//	hitboxes.push_back(new Hitbox(hitbox, attacks["specialU"], 1, OnHitData(6, false, false)));
	//}
	else if (frameNumber == attacks["specialU"].startUp)
	{
		anim->StartAnimation("especialU");
		b2Vec2 a;
		if (input->left()) {
			a = b2Vec2(-7, -7);
			a.Normalize();
			a *= 7;
		}
		else if (input->right())
		{
			a = b2Vec2(7, -7);
			a.Normalize();
			a *= 7;
		}
		else {
			a = b2Vec2(0, -7);
			a.Normalize();
			a *= 7;
		}
		body->SetTransform(body->GetPosition() + a, 0);
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, -25 });
		body->SetGravityScale(10.0f);
		dash = false;

		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialU"], 1, OnHitData(6, false, false)));

	}
	else if (frameNumber == attacks["specialU"].startUp+5)
	{
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
		anim->StartAnimation("especialDEntrada");
		sdl->soundEffects().at("catSpecD").play();
		moving = false;
		blinks -= 1.0f;
	}
	else if (frameNumber == attacks["specialD"].startUp)
	{
		anim->StartAnimation("entrarTP");
		dash = true;	
		counter = true;
	}
	else if (frameNumber == attacks["specialD"].totalFrames-8)
	{
		anim->StartAnimation("salirTP");
	}
	else if (frameNumber == attacks["specialD"].totalFrames) {
		anim->StartAnimation("especialDSalida");
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
		moving = false;
		anim->StartAnimation("especialL");
	}
	else if (frameNumber == attacks["specialLHit"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialLHit"], 1, OnHitData(20, false, false)));
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
}


bool GatoEspia::GetHit(attackData a, Entity* attacker)
{
	if (counter) {
		dir = attacker->GetDir();
		body->SetTransform(attacker->GetBody()->GetPosition() + b2Vec2(-dir, 0), 0);
		currentMove = [this](int f) { Counter(f); };
		moveFrame = -1;
		return false;
	}
	if (Character::GetHit(a, attacker)) {
		body->SetGravityScale(10.f);
	}
}

void GatoEspia::Counter(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("salirTP");
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, 0 });
		dash = false;
		counter = false;
	}
	else if (frameNumber == attacks["specialLHit"].startUp)
	{
		anim->StartAnimation("especialL");
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialLHit"], 1, OnHitData(20, false, false)));
	}
	else if (frameNumber == attacks["specialLHit"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}

}


