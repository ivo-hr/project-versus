#include "../../Utils/AnimationManager.h"
#include "Togo.h"
#include "Spear.h"
#include "../../../utils/CheckML.h"
#include "../../Utils/Particle.h"

Spear::Spear(FightManager* manager, b2Vec2 pos, HitData attack, b2Vec2 dir, Togo* togo) :
	Projectile(manager, pos, dir, 5.25f, 0.7f, 20)
{
	owner = togo;
	texture = &sdl->images().at("dinoSouls");
	//funciona , but i dont know why
	data = attack;

	duration = 240;

	lag = 0;

	body->SetGravityScale(8.0f);

	hitDelay = 5;

	body->GetUserData().pointer = 1; // Para atravesar el suelo
}

Spear::~Spear()
{

}

void Spear::update()
{
	outFor++;
	if (outFor <= duration)
	{
		body->SetLinearVelocity(vecDir);

		hurtbox.x = manager->b2ToSDLX(body, width);
		hurtbox.y = manager->b2ToSDLY(body, height);

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
	if (hitDelay < 5)
	{
		hitDelay++;
	}
	else
	{
		for (int j = 0; j < oponents.size(); j++)
		{
			SDL_Rect hitArea;
			if (SDL_IntersectRect(&hurtbox, oponents[j]->GetHurtbox(), &hitArea))
			{
				if (oponents[j]->GetHit(data, this))
				{
					hitDelay = 0;
					manager->HitLag(lag);

					oponents[j]->AddParticle(new Particle(
						Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
						1, "sHitParticle", oponents[j]));
				}
			}
		}
	}
	
}

void Spear::OnDeath() {
	if (owner != nullptr) {
		owner->SetSpear(true);	
	}
	Projectile::OnDeath();
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

	SDL_Rect src = { 393 + sprite, 395, 80, 9};
	if (dir < 0)
		texture->render(src, aux, ang, nullptr, SDL_FLIP_HORIZONTAL);
	else
		texture->render(src, aux);

#ifdef _DEBUG

	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG

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


