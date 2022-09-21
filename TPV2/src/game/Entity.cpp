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
	isHit.clear();
}

void Entity::setLastCharacer(Entity* chrcter)
{
	lastCharacter = chrcter;
}

void Entity::SetShake(Vector2D dir, ushort value)
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

SDL_Rect* Entity::GetHurtbox()
{
	return &hurtbox;
}

void Entity::AddHitLag(ushort lag)
{
	hitLag = lag;
	body->SetEnabled(false);
}
