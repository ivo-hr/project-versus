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

void GatoEspia::draw()
{
	Character::draw();
}

//Lo mismo que el de arriba pero mas rapido y debil xd
void GatoEspia::BasicNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN");
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
	}
	else if (frameNumber == attacks["basicF"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 50;

		body->SetLinearVelocity(b2Vec2(dir*20, 0));

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
	}
	else if (frameNumber == attacks["basicU"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 3;
		hitbox.y -= 45;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 1, OnHitData(5, false, false)));
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
	}
	else if (frameNumber == attacks["specialN"].startUp)
	{
		auto bullet = new Bullet(manager, new Vector2D(body->GetPosition().x, body->GetPosition().y),attacks["specialN"], dir);
		manager->AddEntity(bullet);
		bullet->SetOponents(oponents);
	}
	else if (frameNumber == attacks["specialN"].totalFrames)
	{
		//La cadena de ifs para cambiar a donde está apuntando (hay una parecida en cada variante)
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
				currentMove = [this](int f) { SpecialNeutralU(f); };
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
	}
	else if (frameNumber == attacks["specialN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 3;
		hitbox.y -= 45;

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialN"], 1, OnHitData(5, false, false)));
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
	}
	else if (frameNumber == attacks["specialN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 3;
		hitbox.y -= 45;

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialN"], 1, OnHitData(5, false, false)));
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
		anim->StartAnimation("entrarTP");
		moving = false;
	}
	else if (frameNumber == attacks["specialL"].startUp / 2)
	{
		dash = true;
	}
	else if (frameNumber == attacks["specialL"].startUp)
	{
		body->SetTransform(body->GetPosition() + b2Vec2(dir * 7, 0),0);
		anim->StartAnimation("salirTP");
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, 0 });
		dash = false;

		if (input->special())
		{
			currentMove = [this](int f) { TpAtack(f); };
			moveFrame = -5;
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
		anim->StartAnimation("entrarTP");
		moving = false;
	}
	else if (frameNumber == attacks["specialU"].startUp / 2)
	{
		dash = true;
	}
	//No me pregunten por que pero tengo que poner esto para que se vea bienxd
	else if (frameNumber == attacks["specialU"].startUp - 2)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);
		hitboxes.push_back(new Hitbox(hitbox, attacks["specialU"], 1, OnHitData(6, false, false)));
	}
	else if (frameNumber == attacks["specialU"].startUp - 1)
	{
		anim->StartAnimation("especialU");
		body->SetTransform(body->GetPosition() + b2Vec2(0, -7), 0);
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, -25 });
		dash = false;

		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitboxes.push_back(new Hitbox(hitbox, attacks["specialU"], 1, OnHitData(6, false, false)));
	}
	else if (frameNumber == attacks["specialL"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void GatoEspia::SpecialDownward(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("especialDEntrada");
		moving = false;
	}
	else if (frameNumber == attacks["specialD"].startUp)
	{
		dash = true;
	}
	else if (frameNumber == attacks["specialD"].totalFrames)
	{
		anim->StartAnimation("especialDSalida");
		dash = false;
	}
	else if (frameNumber == attacks["specialD"].totalFrames + 30)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
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


