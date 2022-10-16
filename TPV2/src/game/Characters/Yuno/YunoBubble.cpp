#include "YunoBubble.h"
#include "../../Utils/InputConfig.h"
#include "../../../utils/CheckML.h"


YunoBubble::YunoBubble(FightManager* manager, b2Vec2 pos, Yuno* owner, InputConfig* input) : Entity(manager, pos, 1.f, 1.f), yuno(owner), hndlr(input)
{
	body->SetGravityScale(0.f);

	texture = &sdl->images().at("bubble");

	texSrc = { 0, 0, 0, 0 };
	texDest = { 0, 0, 0, 0 };

	arrowsTex = &sdl->images().at("arrows");
	arrowSrc = { 0, arrowsTex->height() * yuno->GetPNumber() / 4, arrowsTex->width(), arrowsTex->height() / 4 };
}

YunoBubble::~YunoBubble()
{
}

void YunoBubble::update()
{
	if (hndlr)
	{
		if (hndlr->up())
		{
			body->ApplyLinearImpulseToCenter(b2Vec2(0, -1), true);
		}
		if (hndlr->down())
		{
			body->ApplyLinearImpulseToCenter(b2Vec2(0, 1), true);
		}
		if (hndlr->left())
		{
			body->ApplyLinearImpulseToCenter(b2Vec2(-1, 0), true);
		}
		if (hndlr->right())
		{
			body->ApplyLinearImpulseToCenter(b2Vec2(1, 0), true);
		}
	}

	Entity::update();
}

void YunoBubble::draw(SDL_Rect* camera)
{
	if (alive)
	{
		if (bubbledEntity)
		{
			texDest = { hurtbox.x + (int)(hurtbox.w * 0.25f), hurtbox.y + (int)(hurtbox.h * 0.25f), (int)(hurtbox.w * 0.5f), (int)(hurtbox.h * 0.5f) };

			SDL_Rect aux = texDest;

			aux.x -= camera->x;
			aux.x *= (manager->GetActualWidth() / (float)camera->w);

			aux.y -= camera->y;
			aux.y *= (manager->GetActualHeight() / (float)camera->h);

			aux.w *= (manager->GetActualWidth() / (float)camera->w);
			aux.h *= (manager->GetActualHeight() / (float)camera->h);

			bubbledEntity->getTexture()->render(texSrc, aux);

		}

		SDL_Rect aux = hurtbox;

		aux.x -= camera->x;
		aux.x *= (manager->GetActualWidth() / (float)camera->w);

		aux.y -= camera->y;
		aux.y *= (manager->GetActualHeight() / (float)camera->h);

		aux.w *= (manager->GetActualWidth() / (float)camera->w);
		aux.h *= (manager->GetActualHeight() / (float)camera->h);

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
}

bool YunoBubble::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake)
{
	if (attacker->isProjectile() && !bubbledEntity)
	{
		GetInsideBubble(attacker);
	}
	else
	{
		Pop();
	}
	return false;
}

void YunoBubble::OnDeath()
{
	Pop();
	Entity::OnDeath();
}

void YunoBubble::GetInsideBubble(Entity* ent)
{
	if (ent->isProjectile())
	{
		ent->changeDir();
		manager->ChangeEntityLayer(ent, layer);
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

		bubbledEntity->GetBody()->SetEnabled(true);

		bubbledEntity->GetBody()->SetTransform(body->GetPosition(), 0);

		bubbledEntity = nullptr;
	}

	yuno->BubblePopped();

	toDelete = true;
}
