#include "Entity.h"

Entity::Entity(b2World* world, SDLUtils* sdl, SDL_Texture* texture)
{
	this->sdl = sdl;

	//Definimos un objeto (dinámico)

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(40.0f, 42.0f);
	groundBodyDef.type = b2_dynamicBody;

	//Definimos un caja
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width / 2, height / 2);

	//Creamos una "cuerpo" 
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 10.f;
	fixtureDef.friction = 0.9f;

	body = world->CreateBody(&groundBodyDef);
	//añadimos el cuerpo al objeto fisico
	body->CreateFixture(&fixtureDef);

	body->SetFixedRotation(true);

	body->SetGravityScale(10.f);

	body->SetLinearDamping(0.f);

	onGround = true;



	//Tamaño de la hurtbox del personaje
	hurtbox = { 0,
		0,
		(int)(width * 20.f), (int)(height * 20.f) };

	//creamos el detector de colisiones
	world->SetContactListener(&listener);
}

Entity::~Entity()
{

}

void Entity::update()
{
	//Actualizamos la posicion del rect
	hurtbox.x = body->GetPosition().x * 20 - width * 20 / 2;
	hurtbox.y = body->GetPosition().y * 20 - height * 20 / 2;
}

/*void Entity::SetGround(bool ground)
{
	onGround = ground;
}*/

void Entity::draw()
{
	//if (debug)
	SDL_SetRenderDrawColor(sdl->renderer(), 0, 0, 255, 255);
	SDL_RenderDrawRect(sdl->renderer(), &hurtbox);

	//dibujar los sprite bruh
}

SDL_Rect* Entity::GetHurtbox()
{
	return &hurtbox;
}
