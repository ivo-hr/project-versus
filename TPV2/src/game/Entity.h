#pragma once

#include <string>

#include "../../../box2d/include/box2d/box2d.h"
#include <SDL.h>
#include "../sdlutils/SDLUtils.h"

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
		if (one->GetType() != two->GetType())
		{
			//character.SetGround(true);
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


struct atackData {
	int damage;
	float multiplier;
};

class Entity
{

protected:

	SDLUtils* sdl;

	b2Body* body;

	SDL_Rect hurtbox;
	float width = 2.5f;
	float height = 2.5f;

	bool onGround;

	myListener listener;
public:

	Entity(b2World* world, SDLUtils* sdl);
	~Entity();

	virtual void update();
	virtual void draw();

	virtual void GetHit(atackData a, int dir) = 0;
	virtual SDL_Rect* GetHurtbox();
	bool GetGround() { return onGround; };
	void SetGround(bool ground);
};