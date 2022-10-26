#include "../../Utils/AnimationManager.h"
#include "Spell.h"
#include "../../../utils/CheckML.h"

Spell::Spell(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, state state) :
	Projectile(manager, pos, dir, 2.f, 1.f, 20)
{
	
	if (state == fire)
	{
		texture = &sdl->images().at("nasnasFire");
	}
	else if (state == water)
	{
		texture = &sdl->images().at("nasnasWater");
	}
	else if (state == electric)
	{
		texture = &sdl->images().at("nasnasElectric");
	}

	data = attack;

	duration = 200;

	body->GetUserData().pointer = 1; // Para atravesar el suelo
}

Spell::~Spell()
{

}

void Spell::draw(SDL_Rect* camera)
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

	SDL_Rect src = { 0 + sprite, 1264, 35, 16 };

	if (dir < 0)
		texture->render(src, aux, 0, nullptr, SDL_FLIP_HORIZONTAL);
	else
		texture->render(src, aux);

#ifdef _DEBUG

	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG

	if (anim >= 1) {
		sprite += 200;
		if (sprite == 1200) {
			sprite = 0;
		}
	}
	anim += 0.1f;
}