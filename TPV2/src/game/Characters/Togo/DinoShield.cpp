#include "DinoShield.h"
#include "Togo.h"
#include "../../PlayingState/FightManager.h"
#include "../../../utils/CheckML.h"

DinoShield::DinoShield(FightManager* manager, b2Vec2 pos) :
	Entity(manager, pos, 7.5f, 5.f)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	texture = &sdl->images().at("dinoSouls");

	AddTag(Tags::Hitable);
}

DinoShield::~DinoShield()
{

}

void DinoShield::update()
{
}


void DinoShield::draw()
{
	texture->render(hurtbox);
}

void DinoShield::draw(SDL_Rect* camera)
{
	SDL_Rect aux = hurtbox;

	float wDiff = (float)manager->GetActualWidth() / (float)camera->w;
	float hDiff = (float)manager->GetActualHeight() / (float)camera->h;

	aux.x -= camera->x;
	aux.x = (int)((float)aux.x * wDiff);

	aux.y -= camera->y;
	aux.y = (int)((float)aux.y * hDiff);

	aux.w = (int)((float)aux.w * wDiff);
	aux.h = (int)((float)aux.h * hDiff);

	SDL_Rect src = { 896 - spriteX, 903 + spriteY, 128, 82};

	if (dir < 0)
		texture->render(src, aux);
	else
		texture->render(src, aux, 0, nullptr, SDL_FLIP_HORIZONTAL);

#ifdef _DEBUG

	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG

	if (anim >= 1) {
		if (spriteX == 0) {
			spriteX = 896;
			spriteY = 82;
		}
		else {
			spriteX = 0;
			spriteY = 0;
		}
		anim = 0;
	}
	anim += 0.1f;

}

bool DinoShield::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake)
{
	if (attacker->HasTag(Tags::IsProjectile))
	{
		attacker->changeDir();
		manager->ChangeEntityLayer(attacker, layer);
	}
	return false;
}
