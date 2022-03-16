#include "Entity.h"
#include "Utils/AnimationManager.h"

Entity::Entity(FightManager* mngr, Vector2D* position) : manager(mngr)
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
	fixtureDef.filter.categoryBits = 1;
	fixtureDef.filter.maskBits = 2; // Colisiona con el suelo (tiene este categoryBits en FightManager)

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

SDL_Rect* Entity::GetHurtbox()
{
	return &hurtbox;
}
