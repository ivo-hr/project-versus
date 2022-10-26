#include "YunoBubble.h"
#include "../../Utils/InputConfig.h"
#include "../../../utils/CheckML.h"


YunoBubble::YunoBubble(FightManager* manager, b2Vec2 pos, Yuno* owner, InputConfig* input) : Entity(manager, pos, 1.5f, 1.5f), yuno(owner), hndlr(input)
{
	body->SetGravityScale(0.f);

	texture = &sdl->images().at("bubble");

	texSrc = { 0, 0, 0, 0 };
	texDest = { 0, 0, 0, 0 };

	arrowsTex = &sdl->images().at("arrows");
	arrowSrc = { 0, arrowsTex->height() * yuno->GetPNumber() / 4, arrowsTex->width(), arrowsTex->height() / 4 };

	dir = yuno->GetDir();

	manager->FollowCamera(this);
	AddTag(Tags::Hitable);

	alive = true;
}

YunoBubble::~YunoBubble()
{
	Pop();
}

void YunoBubble::update()
{
	if (hndlr)
	{
		if (hndlr->up())
		{
			body->ApplyLinearImpulseToCenter(b2Vec2(0, -2), true);
		}
		if (hndlr->down())
		{
			body->ApplyLinearImpulseToCenter(b2Vec2(0, 2), true);
		}
		if (hndlr->left())
		{
			body->ApplyLinearImpulseToCenter(b2Vec2(-2, 0), true);
		}
		if (hndlr->right())
		{
			body->ApplyLinearImpulseToCenter(b2Vec2(2, 0), true);
		}
	}
	else
	{
		body->SetLinearVelocity(b2Vec2((float)(dir * 5), 0.f));
	}

	if (bubbledEntity)
		bubbledEntity->GetBody()->SetTransform(body->GetPosition(), 0);

	if (bubbledEntity && bubbledEntity->ToDelete())
	{
		delete bubbledEntity;
		bubbledEntity = nullptr; 
		setToDelete();
	}

	Entity::update();
}

void YunoBubble::draw(SDL_Rect* camera)
{
	if (alive)
	{
		if (bubbledEntity)
		{
			texDest = { hurtbox.x + (int)(hurtbox.w * 0.1f), hurtbox.y + (int)(hurtbox.h * 0.1f), (int)(hurtbox.w * 0.8f), (int)(hurtbox.h * 0.8f) };

			SDL_Rect aux = texDest;

			float wDiff = (float)manager->GetActualWidth() / (float)camera->w;
			float hDiff = (float)manager->GetActualHeight() / (float)camera->h;

			aux.x -= camera->x;
			aux.x = (int)((float)aux.x * wDiff);

			aux.y -= camera->y;
			aux.y = (int)((float)aux.y * hDiff);

			aux.w = (int)((float)aux.w * wDiff);
			aux.h = (int)((float)aux.h * hDiff);

			bubbledEntity->getTexture()->render(texSrc, aux);

		}

		SDL_Rect aux = hurtbox;

		float wDiff = (float)manager->GetActualWidth() / (float)camera->w;
		float hDiff = (float)manager->GetActualHeight() / (float)camera->h;

		aux.x -= camera->x;
		aux.x = (int)((float)aux.x * wDiff);

		aux.y -= camera->y;
		aux.y = (int)((float)aux.y * hDiff);

		aux.w = (int)((float)aux.w * wDiff);
		aux.h = (int)((float)aux.h * hDiff);

		texture->render(aux);

		int xpos = aux.x + (aux.w / 2);
		arrowsTex->render(arrowSrc, { xpos - 15, aux.y - 34, 30, 16 });

#ifdef _DEBUG

		SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG

		/*if (anim >= 1) {
			if (sprite != 320) {
				sprite += 140;
			}
			else {
				sprite = 0;
			}
			anim = 0;
		}
		anim += 0.1;*/
	}
}

void YunoBubble::CheckHits()
{
	if (alive && !bubbledEntity && !ToDelete())
	{
		Entity* oponent = nullptr;
		while (manager->GetNextEntity(oponent, yuno->GetLayer()))
		{
			if (oponent->GetName() != this->GetName())
			{
				if (SDL_HasIntersection(&hurtbox, oponent->GetHurtbox()))
				{
					GetInsideBubble(oponent);
				}
			}
		}
	}
}

bool YunoBubble::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake)
{
	if (attacker->HasTag(Tags::IsProjectile) && !bubbledEntity)
	{
		GetInsideBubble(attacker);
	}
	else
	{
		setToDelete();
	}
	return false;
}

void YunoBubble::OnDeath()
{
	yuno->BubblePopped();
	Entity::OnDeath();
}

void YunoBubble::GetInsideBubble(Entity* ent)
{
	if (toDelete)
	{
		return;
	}

	if (ent->HasTag(Tags::IsProjectile))
	{
		ent->changeDir();
		manager->ChangeEntityLayer(ent, yuno->GetLayer());
	}
	else if (ent->HasTag(Tags::IsCharacter))
	{
		auto chr = static_cast<Character*>(ent);
		chr->ResetChar();
	}

	bubbledEntity = ent;
	manager->RemoveEntity(bubbledEntity, false);

	bubbledEntity->GetBody()->SetEnabled(false);
	bubbledEntity->GetHurtbox()->x = -100;

	texSrc = bubbledEntity->getCurrentSpriteSrc();
}

void YunoBubble::Pop()
{
	if (bubbledEntity)
	{
		manager->AddEntity(bubbledEntity);

		if (bubbledEntity->HasTag(Tags::CameraFollow))
		{
			manager->FollowCamera(bubbledEntity);
		}

		bubbledEntity->GetBody()->SetEnabled(true);

		bubbledEntity->GetBody()->SetLinearVelocity(b2Vec2(0, -1));
	}

	toDelete = true;
}
