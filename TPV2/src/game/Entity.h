#pragma once

#include "PlayingState/FightManager.h"

class AnimationManager;

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

	AnimationManager* anim;

	Texture* texture;

	FightManager* manager;

	SDLUtils* sdl;

	b2Body* body;

	SDL_Rect hurtbox;
	float width = 6.f;
	float height = 6.f;

	std::vector<Entity*> oponents;

	bool onGround;

	myListener listener;
public:


	Entity(FightManager* mngr); 
	~Entity();

	virtual void update();
	virtual void draw();

	virtual void SetOponents(std::vector<Entity*> op);

	virtual void GetHit(atackData a, int dir) = 0;
	virtual SDL_Rect* GetHurtbox();
	//bool GetGround() { return onGround; };
	//void SetGround(bool ground);
};