#include "../../Utils/AnimationManager.h"
#include "Spell.h"
#include "../../../utils/CheckML.h"

Spell::Spell(FightManager* manager, b2Vec2 pos, attackData attack, b2Vec2 dir, state state) :
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

	//si hurtbox.x = camera w + camera x                   aux.x = manager->GetActualWidth()
	//   hurtbox.x = camera w / 2 + camera x               aux.x = manager->GetActualWidth() / 2

	aux.x -= camera->x;
	aux.x *= (manager->GetActualWidth() / (float)camera->w);

	aux.y -= camera->y;
	aux.y *= (manager->GetActualHeight() / (float)camera->h);

	aux.w *= (manager->GetActualWidth() / (float)camera->w);
	aux.h *= (manager->GetActualHeight() / (float)camera->h);

	SDL_Rect src = { 0 + sprite, 1264, 35, 16 };
	if (dir < 0)
		texture->render(src, aux, ang, nullptr, SDL_FLIP_HORIZONTAL);
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
	anim += 0.1;
}