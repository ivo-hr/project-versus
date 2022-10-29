#include "Entity.h"
#include "Utils/Particle.h"
#include "../utils/CheckML.h"


void Entity::BuildParticlePool()
{
	for (ushort i = 0; i < 2; i++)
	{
		particlePool["smallHit"].push_front(new Particle({ 0,0 }, "sHitParticle", this, 1));
		particlePool["bigHit"].push_front(new Particle({ 0,0 }, "bHitParticle", this, 1));
		particlePool["run"].push_front(new Particle({ 0,0 }, "run", this, 1));
		particlePool["water"].push_front(new Particle({ 0,0 }, "water", this, 1));
		particlePool["electric"].push_front(new Particle({ 0,0 }, "electric", this, 1));
		particlePool["fire"].push_front(new Particle({ 0,0 }, "fire", this, 1));
	}
}

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

	BuildParticlePool();
}

Entity::~Entity()
{
	manager->GetWorld()->DestroyBody(body);

	for (auto i = particlePool.begin(); i != particlePool.end(); i++)
	{
		for (ushort j = 0; j < i->second.size(); j++)
		{
			delete i->second[j];
		}
		i->second.clear();
	}
	particlePool.clear();
}

void Entity::updateParticles()
{
	ushort maxSize = (ushort)max(backParticles.size(), frontParticles.size());

	for (ushort i = 0u; i < maxSize; i++)
	{
		if (i < backParticles.size())
		{
			backParticles[i]->update();
			if (!backParticles[i]->IsActive())
			{
				RemoveParticle(backParticles[i], i, false);
				if (backParticles.size() > 0)
					i--;
			}
		}

		if (i < frontParticles.size())
		{
			frontParticles[i]->update();
			if (!frontParticles[i]->IsActive())
			{
				RemoveParticle(frontParticles[i], i, true);
				if (frontParticles.size() > 0)
					i--;
			}
		}
	}
}


void Entity::update()
{
	if (hitLag > 0)
	{
		hitLag--;
		if (hitLag == 0)
		{
			body->SetEnabled(true);
		}
		return;
	}

	if (body->IsEnabled())
	{
		hurtbox.x = manager->b2ToSDLX(body, width);
		hurtbox.y = manager->b2ToSDLY(body, height);
	}
	else
	{
		hurtbox.x = -100;
	}

	if (IsOutOfBounds())
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

#ifdef _DEBUG

	for (int i = 0; i < hitboxes.size(); i++)
	{
		SDL_Rect aux = hitboxes[i]->box;

		float wDiff = (float)manager->GetActualWidth() / (float)camera->w;
		float hDiff = (float)manager->GetActualHeight() / (float)camera->h;

		aux.x -= camera->x;
		aux.x = (int)((float)aux.x * wDiff);

		aux.y -= camera->y;
		aux.y = (int)((float)aux.y * hDiff);

		aux.w = (int)((float)aux.w * wDiff);
		aux.h = (int)((float)aux.h * hDiff);

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &aux);
	}

#endif // _DEBUG
}

void Entity::AddParticle(const string& name, const Vector2D& pos, short dir, bool front)
{
	if (particlePool[name].empty())
		return;
	Particle* aux = particlePool[name].front();
	if (aux->IsActive())
		aux->Deactivate();
	if (name == "run")
		aux->Activate(pos, dir, true);
	else
		aux->Activate(pos, dir);
	particlePool[name].pop_front();
	particlePool[name].push_back(aux);

	if (front)
	{
		frontParticles.push_back(aux);
	}
	else
	{
		backParticles.push_back(aux);
	}
}

void Entity::RemoveParticle(Particle* par, ushort posInVec, bool front)
{
	par->Deactivate();
	if (front)
	{
		frontParticles.erase(frontParticles.begin() + posInVec);
	}
	else
	{
		backParticles.erase(backParticles.begin() + posInVec);
	}
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
