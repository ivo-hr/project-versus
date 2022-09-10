#include "Entity.h"
#include "Utils/Particle.h"
#include "../utils/CheckML.h"


Entity::Entity(FightManager* mngr, b2Vec2 position, float w, float h) : manager(mngr), width(w), height(h)
{
	this->sdl = mngr->GetSDLU();

	//Definimos un objeto (dinámico)

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(position.x, position.y);
	groundBodyDef.type = b2_dynamicBody;

	//Definimos un caja
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width / 2, height / 2);

	//Creamos una "cuerpo" 
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 3.f;
	fixtureDef.friction = 0.0f; // Para que no se pegue a las paredes
	fixtureDef.filter.categoryBits = 1; // 1 para los personajes (se atraviesan entre sí)
	fixtureDef.filter.maskBits = 2 | 4; // Colisiona con el suelo y plataforma (tiene este categoryBits en FightManager)

	body = mngr->GetWorld()->CreateBody(&groundBodyDef);
	//añadimos el cuerpo al objeto fisico
	body->CreateFixture(&fixtureDef);

	body->SetBullet(true);

	body->SetFixedRotation(true);

	body->SetGravityScale(10.f);

	body->SetLinearDamping(0.f);

	dynamicBox.SetAsBox(width * 0.45f, 0.05f, b2Vec2(0, + height / 2), 0);
	fixtureDef.isSensor = true;
	b2Fixture* footSensorFixture = body->CreateFixture(&fixtureDef);

	onGround = false;

	dir = 1;

	respawnPos = position;

	//Tamaño de la hurtbox del personaje
	hurtbox = mngr->GetSDLCoors(body, width, height);

}

Entity::~Entity()
{
	for (Particle* ent : particulas) RemoveParticle(ent);
	manager->GetWorld()->DestroyBody(body);
}

void Entity::updateParticles()
{
	for (Particle* part : particulas)
	{
		part->update();
	}

	for (auto i = 0u; i < particulas.size(); i++)
	{
		if (particulas[i]->dead)
		{
			RemoveParticle(particulas[i]);
			if (particulas.size() > 0)
				i--;
		}
	}
}

void Entity::update()
{
	updateParticles();

	if (hitLag > 0)
	{
		hitLag--;
		if (hitLag == 0)
		{
			body->SetEnabled(true);
		}
		return;
	}

	//Actualizamos la posicion del rect
	hurtbox.x = manager->b2ToSDLX(body, width);
	hurtbox.y = manager->b2ToSDLY(body, height);

	if (!SDL_HasIntersection(&hurtbox, manager->GetDeathZone()))
	{
		OnDeath();
	}
}

void Entity::SetGround(bool ground)
{
	onGround = ground;
}

void Entity::resetHit()
{
	for (int i = 0; i < isHit.size(); i++) {
		isHit[i] = false;
	}
}

void Entity::setLastCharacer(Entity* chrcter)
{
	lastCharacter = chrcter;
}

void Entity::SetShake(Vector2D dir, uint16 value)
{
	shakeValue = dir * value;
}

void Entity::draw()
{
	for (Particle* ent : particulas)
	{
		ent->draw();
	}

#ifdef _DEBUG

	for (int i = 0; i < hitboxes.size(); i++)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitboxes[i]->box);
	}

#endif // _DEBUG
}

void Entity::draw(SDL_Rect* camera)
{
	for (Particle* ent : particulas)
	{
		ent->draw(camera);
	}

#ifdef _DEBUG

	for (int i = 0; i < hitboxes.size(); i++)
	{
		SDL_Rect aux = hitboxes[i]->box;

		aux.x -= camera->x;
		aux.x *= (manager->GetActualWidth() / (float)camera->w);

		aux.y -= camera->y;
		aux.y *= (manager->GetActualWidth() / (float)camera->w);

		aux.w *= (manager->GetActualWidth() / (float)camera->w);
		aux.h *= (manager->GetActualWidth() / (float)camera->w);

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &aux);
	}

#endif // _DEBUG
}

void Entity::AddParticle(Particle* par)
{
	particulas.push_back(par);
}

bool Entity::RemoveParticle(Particle* par)
{
	for (int i = 0; i < particulas.size(); i++)
	{
		if (particulas[i] == par)
		{
			for (int j = i + 1; j < particulas.size(); j++)
			{
				particulas[j - 1] = particulas[j];
			}
			particulas.pop_back();
		}
	}
	delete par;
	return false;
}

//Le decimos a quien toca dar de ostias xd
void Entity::SetOponents(std::vector<Entity*> ents)
{
	oponents.clear();
	isHit.clear();
	for (int i = 0; i < ents.size(); i++)
	{
		if (ents[i] != this)
		{
			oponents.push_back(ents[i]);
			isHit.push_back(false);
		}
	}
}

void Entity::AddOponent(Entity* ent)
{
	if (ent != this)
	{
		oponents.push_back(ent);
		isHit.push_back(false);
	}
}

void Entity::DeleteOponent(Entity* ent)
{
	for (int i = 0; i < oponents.size(); i++)
	{
		if (oponents[i] == ent)
		{
			for (int j = i + 1; j < oponents.size(); j++)
			{
				oponents[j - 1] = oponents[j];
				isHit[j - 1] = isHit[j];
			}
			oponents.pop_back();
			isHit.pop_back();
		}
	}
}

void Entity::CheckHits()
{
	bool toResetHits = false;

	if (hitLag > 0)
	{
		return;
	}

	for (int i = 0; i < hitboxes.size(); i++)
	{

		if (hitboxes[i]->duration == hitboxes[i]->outFor)
		{
			hitboxes[i]->outFor = 0;
			for (int j = i + 1; j < hitboxes.size(); j++)
			{
				hitboxes[j - 1] = hitboxes[j];
			}
			hitboxes.pop_back();
			i--;
			toResetHits = true;
		}
		else
		{
			hitboxes[i]->box.x = (hurtbox.x + (hurtbox.w / 2) + hitboxes[i]->charOffset.getX()) - hitboxes[i]->box.w / 2;
			hitboxes[i]->box.y = (hurtbox.y + (hurtbox.h / 2) + hitboxes[i]->charOffset.getY()) - hitboxes[i]->box.h / 2;
			hitboxes[i]->outFor++;
		}

		if (!toResetHits)
		for (int j = 0; j < oponents.size(); j++)
		{
			SDL_Rect hitArea;
			if (SDL_IntersectRect(&hitboxes[i]->box, oponents[j]->GetHurtbox(), &hitArea) && !isHit[j])
			{
				manager->MoveToFront(this);
				bool hitLagApplied = false, shakeApplied = false, camShakeApplied = false;
				//Le hace daño xddd
				if (oponents[j]->GetHit(hitboxes[i]->hitdata, this, hitLagApplied, shakeApplied, camShakeApplied))
				{
					if (!hitLagApplied)
					{
						AddHitLag(hitboxes[i]->GetHitlag());
						oponents[j]->AddHitLag(hitboxes[i]->GetHitlag());
					}

					if (!shakeApplied)
					{
						oponents[j]->SetShake(Vector2D(hitboxes[i]->hitdata.direction.x, hitboxes[i]->hitdata.direction.y), hitboxes[i]->GetHitlag());
					}

					if (!camShakeApplied)
					{
						manager->SetShake(Vector2D(hitboxes[i]->hitdata.direction.x * -hitboxes[i]->GetHitlag() * 0.3f, hitboxes[i]->hitdata.direction.y * hitboxes[i]->GetHitlag() * 0.1f), hitboxes[i]->GetHitlag());
					}

					oponents[j]->setLastCharacer(this);

					if (hitboxes[i]->GetHitlag() >= 15)
					{
						AddParticle(new Particle(
							 Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
							1, "bHitParticle", this));

						manager->GetSDLU()->soundEffects().at("hitStr").play();
					}
					else
					{
						AddParticle(new Particle(
							 Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
							1, "sHitParticle", this)); 

						manager->GetSDLU()->soundEffects().at("hitMed").play();
					}
				}
				isHit[j] = true;
			}
		}
	}
	if (toResetHits)
	{
		resetHit();
	}
}

SDL_Rect* Entity::GetHurtbox()
{
	return &hurtbox;
}

void Entity::AddHitLag(uint16 lag)
{
	hitLag = lag;
	body->SetEnabled(false);
}
