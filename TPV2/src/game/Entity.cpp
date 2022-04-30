#include "Entity.h"
#include "Utils/Particle.h"

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
	fixtureDef.friction = 0.9f;
	fixtureDef.filter.categoryBits = 1; // 1 para los personajes (se atraviesan entre sí)
	fixtureDef.filter.maskBits = 2 | 4; // Colisiona con el suelo y plataforma (tiene este categoryBits en FightManager)

	body = mngr->GetWorld()->CreateBody(&groundBodyDef);
	//añadimos el cuerpo al objeto fisico
	body->CreateFixture(&fixtureDef);

	body->SetBullet(true);

	body->SetFixedRotation(true);

	body->SetGravityScale(10.f);

	body->SetLinearDamping(0.f);

	dynamicBox.SetAsBox(width / 3, height / 8, b2Vec2(0, + height / 2), 0);
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
	manager->GetWorld()->DestroyBody(body);
}

void Entity::updateParticles()
{
	for (Particle* ent : particulas)
	{
		ent->update();
	}
}

void Entity::update()
{
	updateParticles();
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
		aux.y *= (manager->GetActualHeight() / (float)camera->h);

		aux.w *= (manager->GetActualWidth() / (float)camera->w);
		aux.h *= (manager->GetActualHeight() / (float)camera->h);

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

	for (int i = 0; i < hitboxes.size(); i++)
	{
		for (int j = 0; j < oponents.size(); j++)
		{
			SDL_Rect hitArea;
			if (SDL_IntersectRect(&hitboxes[i]->box, oponents[j]->GetHurtbox(), &hitArea) && !isHit[j])
			{
				//Le hace daño xddd
				if (oponents[j]->GetHit(hitboxes[i]->data, this))
				{
					manager->HitLag(hitboxes[i]->hit.hitlag);

					if (hitboxes[i]->hit.hitlag >= 15)
					{
						AddParticle(new Particle(
							new Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
							1, "bHitParticle", this));

						manager->GetSDLU()->soundEffects().at("hitStr").play();
					}
					else
					{
						AddParticle(new Particle(
							new Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
							1, "sHitParticle", this));

						manager->GetSDLU()->soundEffects().at("hitMed").play();
					}
				}
				isHit[j] = true;
			}
		}
		hitboxes[i]->duration--;
		if (hitboxes[i]->duration <= 0)
		{
			Hitbox* aux = hitboxes[i];
			for (int j = i + 1; j < hitboxes.size(); j++)
			{
				hitboxes[j - 1] = hitboxes[j];
			}
			hitboxes.pop_back();
			delete aux;
			i--;
			toResetHits = true;
		}
		else
		{
			if (hitboxes[i]->follow)
			{
				hitboxes[i]->box.x = hurtbox.x + hitboxes[i]->charOffset.getX();
				hitboxes[i]->box.y = hurtbox.y + hitboxes[i]->charOffset.getY();
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
