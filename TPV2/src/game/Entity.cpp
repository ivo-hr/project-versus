#include "Entity.h"
#include "Utils/AnimationManager.h"

Entity::Entity(FightManager* mngr, Vector2D* position, float w, float h) : manager(mngr), width(w), height(h)
{
	this->sdl = mngr->GetSDLU();

	//Definimos un objeto (dinámico)

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(position->getX(), position->getY());
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

	body->SetFixedRotation(true);

	body->SetGravityScale(10.f);

	body->SetLinearDamping(0.f);

	onGround = true;

	dir = 1;

	respawnPos = position;

	//Tamaño de la hurtbox del personaje
	hurtbox = mngr->GetSDLCoors(body, width, height);

}

Entity::~Entity()
{

}

void Entity::update()
{
	//Actualizamos la posicion del rect
	hurtbox.x = manager->b2ToSDLX(body, width);
	hurtbox.y = manager->b2ToSDLY(body, height);

	anim->update();

	if (!SDL_HasIntersection(&hurtbox, manager->GetDeathZone()))
	{
		OnDeath();
	}
}

void Entity::SetGround(bool ground)
{
	onGround = ground;
}

void Entity::draw()
{
	anim->render();

	//dibujar los sprite bruh
}

//Le decimos a quien toca dar de ostias xd
void Entity::SetOponents(std::vector<Entity*> ents)
{
	for (int i = 0; i < oponents.size(); i++)
	{
		oponents.pop_back();
	}
	for (int i = 0; i < ents.size(); i++)
	{
		if (ents[i] != this)
		{
			oponents.push_back(ents[i]);
		}
	}
}

void Entity::CheckHits()
{
	for (int i = 0; i < hitboxes.size(); i++)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitboxes[i]->box);

		for (int j = 0; j < oponents.size(); j++)
		{
			if (SDL_HasIntersection(&hitboxes[i]->box, oponents[j]->GetHurtbox()))
			{
				//Le hace daño xddd
				if (oponents[j]->GetHit(hitboxes[i]->data, dir))
				{
					manager->HitLag(hitboxes[i]->hit.hitlag);
				}
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
		}
	}
}

SDL_Rect* Entity::GetHurtbox()
{
	return &hurtbox;
}
