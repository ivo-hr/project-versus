#include "YunoBubble.h"
#include "../../Utils/InputConfig.h"
#include "../../../utils/CheckML.h"
#include "../../Projectile.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../sdlutils/Texture.h"


YunoBubble::YunoBubble(FightManager* manager, b2Vec2 pos, Yuno* owner, int iniSpan, int moreSpan, Bubble bubbleType, InputConfig* input) : Entity(manager, pos, bubbleType == Bubble::UP ? 3.f : 2.f, bubbleType == Bubble::UP ? 3.f : 2.f), yuno(owner), hndlr(input)
{
	body->SetGravityScale(0.f);

	texture = &sdl->images().at("bubble");

	texSrc = { 0, 0, 0, 0 };
	texDest = { 0, 0, 0, 0 };

	arrowsTex = &sdl->images().at("arrows");
	arrowSrc = { 0, 0, arrowsTex->width(), arrowsTex->height() / 4 };
	arrowSrc.y = arrowSrc.h * (yuno->GetLayer() - 1);

	dir = yuno->GetDir();

	manager->FollowCamera(this);
	AddTag(Tags::Hitable);

	alive = true;

	totalLifespan = (float)(iniSpan * 60);
	lifespan = totalLifespan;
	moarSpan = moreSpan;

	pompa = bubbleType;

	b2Vec2* dethZoneOriginal = manager->GetDeathZoneB2();
	bubbleDeathZone = { manager->ToSDL(width), manager->ToSDL(height), (int)(manager->ToSDL(dethZoneOriginal->x) - (manager->ToSDL(width) * 2)), pompa == Bubble::UP ? (int)(manager->ToSDL(dethZoneOriginal->y)) : (int)(manager->ToSDL(dethZoneOriginal->y) - (manager->ToSDL(height) * 2)) };
	
	if (pompa == Bubble::UP)
	{
		GetInsideBubble(yuno);
		invFrames = (float)(iniSpan * 30);
	}
}

YunoBubble::~YunoBubble()
{
	texture->SetTexMod(255, 255, 255);
}

void YunoBubble::update()
{
	if (toExplode)
	{
		Explode();
		return;
	}

	if (pompa != Bubble::FORWARD) {
		if (hndlr)
		{
			if (hndlr->up() && pompa == Bubble::NEUTRAL)
			{
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -3.5f), true);
			}
			if (hndlr->down() && pompa == Bubble::NEUTRAL)
			{
				body->ApplyLinearImpulseToCenter(b2Vec2(0, 3.5f), true);
			}
			if (hndlr->left())
			{
				body->ApplyLinearImpulseToCenter(b2Vec2(-3.5f, 0), true);
			}
			if (hndlr->right())
			{
				body->ApplyLinearImpulseToCenter(b2Vec2(3.5f, 0), true);
			}
			if(pompa == Bubble::UP){
				if (hndlr->special() && timeSinceBubble > 10) {
					Pop();
				}
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -2.5f), true);
			}

		}
		else if (pompa == Bubble::NEUTRAL)
		{
			body->SetLinearVelocity(b2Vec2(dir * 5, 0));
		}
	}

	if (bubbledEntity)
	{
		bubbledEntity->SetPosition(body->GetPosition());

		if (bubbledEntity->ToDelete())
		{
			Pop();
		}
	}

	if (invFrames != 0) {
		invFrames--;
	}
	if (lifespan != 0) {
		lifespan--;
	}
	else
	{
		Pop();
	}
	timeSinceBubble++;

	if (lifespan * 2 == totalLifespan || lifespan * 2 == totalLifespan - 1)
	{
		texture->SetTexMod(255, 64, 64);
	}

	Entity::update();
}

void YunoBubble::draw(const SDL_Rect& camera)
{
	if (alive)
	{
		if (bubbledEntity)
		{
			texDest = { hurtbox.x + (int)(hurtbox.w * 0.1f), hurtbox.y + (int)(hurtbox.h * 0.1f), (int)(hurtbox.w * 0.8f), (int)(hurtbox.h * 0.8f) };

			SDL_Rect aux = texDest;

			float wDiff = (float)manager->GetActualWidth() / (float)camera.w;
			float hDiff = (float)manager->GetActualHeight() / (float)camera.h;

			aux.x -= camera.x;
			aux.x = (int)((float)aux.x * wDiff);

			aux.y -= camera.y;
			aux.y = (int)((float)aux.y * hDiff);

			aux.w = (int)((float)aux.w * wDiff);
			aux.h = (int)((float)aux.h * hDiff);

			if (bubbledEntity->GetDir() > 0)
				bubbledEntity->getTexture()->render(texSrc, aux);
			else
				bubbledEntity->getTexture()->render(texSrc, aux, 0, nullptr, SDL_FLIP_HORIZONTAL);

		}

		SDL_Rect aux = hurtbox;

		float wDiff = (float)manager->GetActualWidth() / (float)camera.w;
		float hDiff = (float)manager->GetActualHeight() / (float)camera.h;

		aux.x -= camera.x;
		aux.x = (int)((float)aux.x * wDiff);

		aux.y -= camera.y;
		aux.y = (int)((float)aux.y * hDiff);

		aux.w = (int)((float)aux.w * wDiff);
		aux.h = (int)((float)aux.h * hDiff);

		texture->render(aux);

		int xpos = aux.x + (aux.w / 2);
		arrowsTex->render(arrowSrc, { xpos - 15, aux.y - 34, 30, 16 });

#ifdef _DEBUG

		SDL_RenderDrawRect(sdl->renderer(), &aux);

		SDL_Rect auxDeathBubble = bubbleDeathZone;

		auxDeathBubble.x -= camera.x;
		auxDeathBubble.x = (int)((float)auxDeathBubble.x * wDiff);

		auxDeathBubble.y -= camera.y;
		auxDeathBubble.y = (int)((float)auxDeathBubble.y * hDiff);

		auxDeathBubble.w = (int)((float)auxDeathBubble.w * wDiff);
		auxDeathBubble.h = (int)((float)auxDeathBubble.h * hDiff);

		SDL_RenderDrawRect(sdl->renderer(), &auxDeathBubble);

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
			if (oponent->GetName() != this->GetName() && oponent->GetName() != "Togo_Shield")
			{
				if (SDL_HasIntersection(&hurtbox, &oponent->GetHurtbox()))
				{
					GetInsideBubble(oponent);
					body->SetLinearVelocity(b2Vec2(dir * 0.5f, 0));
					lifespan += moarSpan * 60;
					totalLifespan = lifespan;

					texture->SetTexMod(255, 255, 255);
				}
			}
			if (bubbledEntity)
			{
				break;
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
		Pop();
	}
	return false;
}

void YunoBubble::OnDeath()
{
	Pop();
}

void YunoBubble::GetInsideBubble(Entity* ent)
{
	if (toDelete)
	{
		return;
	}

	if (ent->HasTag(Tags::IsProjectile))
	{
		auto charac = static_cast<Projectile*>(ent);
		charac->changeDir();
		manager->ChangeEntityLayer(ent, yuno->GetLayer());
	}
	else if (ent->HasTag(Tags::IsCharacter))
	{
		auto chr = static_cast<Character*>(ent);
		chr->ResetChar();
	}

	bubbledEntity = ent;

	bubbledEntity->SetAlive(false);

	bubbledEntity->GetBody()->SetEnabled(false);

	texSrc = bubbledEntity->getCurrentSpriteSrc();
}

void YunoBubble::Pop()
{
	toExplode = true;
}

void YunoBubble::Explode()
{
	if (bubbledEntity)
	{
		bubbledEntity->SetAlive(true);

		bubbledEntity->GetBody()->SetEnabled(true);

		bubbledEntity->SetPosition(body->GetPosition());

		bubbledEntity->GetBody()->SetLinearVelocity(b2Vec2(0, -2));

		if (bubbledEntity->HasTag(Tags::IsCharacter))
		{
			auto chr = static_cast<Character*>(bubbledEntity);
			chr->ResetChar();
			if (bubbledEntity == yuno)
			{
				yuno->setExplotado(true);
				yuno->setRecovery(false);
			}
		}
	}

	yuno->BubblePopped();
	toDelete = true;
}
