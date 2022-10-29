#include "YunoBubble.h"
#include "../../Utils/InputConfig.h"
#include "../../../utils/CheckML.h"


YunoBubble::YunoBubble(FightManager* manager, b2Vec2 pos, Yuno* owner, int iniSpan, int moreSpan, Bubble bubbleType, InputConfig* input) : Entity(manager, pos, bubbleType == Bubble::UP ? 3.f : 2.f, bubbleType == Bubble::UP ? 3.f : 2.f), yuno(owner), hndlr(input)
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

	lifespan = (float)(iniSpan * 60);
	moarSpan = (float)moreSpan;

	pompa = bubbleType;

	b2Vec2* dethZoneOriginal = manager->GetDeathZoneB2();
	bubbleDeathZone = { manager->ToSDL(width), manager->ToSDL(height), (int)(manager->ToSDL(dethZoneOriginal->x) - (manager->ToSDL(width) * 2)), (int)(manager->ToSDL(dethZoneOriginal->y) - (manager->ToSDL(height) * 2)) };
	
	if (pompa == Bubble::UP) {
		GetInsideBubble(yuno);
		invFrames = iniSpan*30;
	}
}

YunoBubble::~YunoBubble()
{
	Pop();
}

void YunoBubble::update()
{
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
					setToDelete();
				}
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -2.5f), true);
			}

		}
		else if (pompa == Bubble::NEUTRAL)
		{
			body->SetLinearVelocity(b2Vec2(dir * 5, 0));
		}
	}

	if (bubbledEntity){
		bubbledEntity->GetBody()->SetTransform(body->GetPosition(), 0);
	}

	if (bubbledEntity && bubbledEntity->ToDelete())
	{
		delete bubbledEntity;
		bubbledEntity = nullptr; 
		setToDelete();
	}

	if (invFrames != 0) {
		invFrames--;
	}
	if (lifespan != 0) {
		lifespan--;
	}
	else {
		setToDelete();
	}
	timeSinceBubble++;

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

		SDL_Rect auxDeathBubble = bubbleDeathZone;

		auxDeathBubble.x -= camera->x;
		auxDeathBubble.x = (int)((float)auxDeathBubble.x * wDiff);

		auxDeathBubble.y -= camera->y;
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
				if (SDL_HasIntersection(&hurtbox, oponent->GetHurtbox()))
				{
					GetInsideBubble(oponent);
					body->SetLinearVelocity(b2Vec2(dir * 0.5f, 0));
					lifespan += moarSpan * 60;
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
		if (bubbledEntity->HasTag(Tags::IsCharacter))
		{
			auto chr = static_cast<Character*>(bubbledEntity);
			if (bubbledEntity == yuno) {
				yuno->setExplotado(true);
				yuno->setRecovery(false);
			}
		}
	}

	toDelete = true;
}
