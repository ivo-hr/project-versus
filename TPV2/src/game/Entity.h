#pragma once

#include "PlayingState/FightManager.h"

class myListener : public b2ContactListener
{
public:
	bool ground = true;
	void BeginContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		if (two->GetType() == b2_staticBody)
		{
			ground = true;
			std::cout << ground << std::endl;
		}
	}
	void EndContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		if (two->GetType() == b2_staticBody)
		{
			std::cout << "ground?" << ground << std::endl;
			ground = false;
			std::cout << "ground?" << ground << std::endl;


		}
	}
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{

	}
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{

	}
	bool CheckGround()
	{
		return ground;
	}
};


struct atackData 
{
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


	Entity(b2World* world, SDLUtils* sdl, SDL_Texture* texture); 
	~Entity();

	virtual void update();
	virtual void draw();

	virtual void GetHit(atackData a, int dir) = 0;
	virtual SDL_Rect* GetHurtbox();
	//bool GetGround() { return onGround; };
	//void SetGround(bool ground);
};