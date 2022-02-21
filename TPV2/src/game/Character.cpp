/*
#include "Character.h"

Character::Character()
{
	//Definimos un objeto (dinámico)
	float width = 5.f;
	float height = 5.f;

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(80.0f, 45.0f);
	groundBodyDef.type = b2_dynamicBody;

	//Definimos un caja
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width / 2, height / 2);

	//Creamos una "cuerpo" 
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 10.f;
	fixtureDef.friction = 0.9f;

	b2Body* body = world.CreateBody(&groundBodyDef);
	//añadimos el cuerpo al objeto fisico
	body->CreateFixture(&fixtureDef);

	body->SetFixedRotation(true);

	body->SetGravityScale(10.f);

	// variables
	damageTaken = 0;
	maxSpeed = 40;
	speed = 0;
	this->jumpCounter = 1;
	this->jumpStr = -1000;

}

Character::~Character()
{

}

void Character::update()
{






}

void Character::atack()
{
}
*/