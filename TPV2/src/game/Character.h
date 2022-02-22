#pragma once

#include <string>

#include "../sdlutils/InputHandler.h"
#include "Entity.h"

<<<<<<< Updated upstream
class myListener : public b2ContactListener
{
private:

public:
	//Character character;
	void BeginContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		std::cout << "contacto" << std::endl;

		if (two->GetType() == b2_staticBody)
		{
			//SetGround(true);
		}
	}
	void EndContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		std::cout << "fin contacto" << std::endl;
		if (one->GetType() != two->GetType())
		{
			//character.SetGround(false);
		}
	}
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		//one->ApplyLinearImpulse(b2Vec2(0, -40), one->GetWorldCenter(), true);
		//two->ApplyLinearImpulse(b2Vec2(0, -40), two->GetWorldCenter(), true);
	}
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{

	}

};

class Character
=======
//Esto no debería ir aquí pero no se como hacerlo
//bool onGround;


class Character : public Entity
>>>>>>> Stashed changes
{
protected:

	int dir = 1;

	bool movable;

	Character* oponent = nullptr;

	InputHandler& ih = *InputHandler::instance();

	//Datos de los ataques (Deberian salir de jsons en un futuro)
	atackData ataqueFuerte;
	atackData ataqueDebil;

	bool moving;
	int maxSpeed;
	int speed;

	//Variables para el salto (fuerza, maximonumero permitido, y el contador)
	int jumpStr;
	int maxJumps;
	int jumpCounter;

	int weight;

	int damageTaken;

	//frame actual del movimiento que este haciendo
	int moveFrame;
	//Metodo del movimiento que este haciendo (esto es una variable que guarda metodos :v)
	void (Character::* currentMove)(int);
public:

	Character(b2World* world, SDLUtils* sdl, bool movable);
	~Character();

	virtual void update() override;
	virtual void atackWeak(int frameNumber);
	virtual void atackStrong(int frameNumber);
	virtual void draw() override;

	virtual void GetHit(atackData a, int dir);
	virtual void SetOponent(Character* op);
	virtual SDL_Rect* GetHurtbox();
	bool GetGround() { return onGround; };
	void SetGround(bool ground);

};