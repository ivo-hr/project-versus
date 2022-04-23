#include "../../Utils/AnimationManager.h"
#include "Togo.h"
#include "Spear.h"
#include "../../Utils/Particle.h"

Spear::Spear(FightManager* manager, Vector2D* pos, attackData attack, b2Vec2 dir, Togo* togo) :
	Projectile(manager, pos, dir, 5.25f, 0.5f, 20)
{
	owner = togo;
	texture = &sdl->images().at("dinoSouls");
	//funciona , but i dont know why
	data = attack;

	range = 30.f;

	lag = 1;

	body->SetGravityScale(8.0f);
}

Spear::~Spear()
{

}

void Spear::update()
{
	float distance = abs(body->GetPosition().x - iniPos->getX());
	if (distance <= range)
	{
		body->SetLinearVelocity(vecDir);

		hurtbox.x = manager->b2ToSDLX(body, width);
		hurtbox.y = manager->b2ToSDLY(body, height);

		updateParticles();

		if (!SDL_HasIntersection(&hurtbox, manager->GetDeathZone()))
		{
			OnDeath();
		}
	}
	else
	{
		OnDeath();
	}
}

void Spear::CheckHits()
{
	for (int j = 0; j < oponents.size(); j++)
	{
		if (SDL_HasIntersection(&hurtbox, oponents[j]->GetHurtbox()))
		{
			SDL_Rect hitArea;
			if (SDL_IntersectRect(&hurtbox, oponents[j]->GetHurtbox(), &hitArea))
			{
				manager->HitLag(lag);
			}
		}
	}
}

void Spear::OnDeath() {
	owner->SetSpear(true);
	manager->RemoveEntity(this);
}

void Spear::draw(SDL_Rect* camera)
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

	SDL_Rect src = { 384 + sprite, 404, 75, 6};
	texture->render(src, aux, ang, nullptr, SDL_FLIP_HORIZONTAL);
	SDL_RenderDrawRect(sdl->renderer(), &aux);
	if (anim >= 1) {
		if (sprite == 0) {
			sprite = 128;
		}
		else {
			sprite = 0;
		}
		anim = 0;
	}
	anim += 0.1;
}


