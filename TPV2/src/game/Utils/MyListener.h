#include <string>
 #include "../../sdlutils/InputHandler.h"
#include <box2d.h>
#include "../Entity.h"
#include <vector>

class MyListener : public b2ContactListener
{
private:
	std::vector<Entity*> characters;

public:
	void BeginContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		bool sensor;

		if (one->GetType() == b2_staticBody) sensor = contact->GetFixtureB()->IsSensor();
		else {
			swap(one, two);
			sensor = contact->GetFixtureA()->IsSensor();
		}
		// Mira si está en contacto con el suelo
		if (one->GetType() == b2_staticBody && sensor)
		{
			for (int i = 0; i < characters.size(); i++)
			{
				if (two == characters[i]->GetBody()/* && two->GetLinearVelocity().y > -0.1f*/)
				{
					characters[i]->SetGround(true);
				}
			}
		}
	}
	void EndContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		bool sensor;

		if (one->GetType() == b2_staticBody) sensor = contact->GetFixtureB()->IsSensor();
		else {
			swap(one, two);
			sensor = contact->GetFixtureA()->IsSensor();
		}

		// Mira si deja de contactar con el suelo
		if (one->GetType() == b2_staticBody && sensor)
		{
			for (int i = 0; i < characters.size(); i++)
			{
				if (two == characters[i]->GetBody())
				{
					characters[i]->SetGround(false);
				}
			}
		}
	}
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		b2Vec2 normalToCheckFor = { 0, -1 };

		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();

		if (!one->GetType() == b2_staticBody)
		{
			swap(one, two);
			normalToCheckFor.y *= -1;
		}
		// Mira si es proyectil o personaje que quiere subir a la plataforma (atravesándola)
		if (one->GetType() == b2_staticBody && (two->GetUserData().pointer == 1 || 
			(one->GetFixtureList()->GetFilterData().categoryBits == 4 && contact->GetManifold()->localNormal != normalToCheckFor)))
		{ 
			contact->SetEnabled(false);
		}
	}
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{

	}
	void AddCharacter(Entity* character)
	{
		characters.push_back(character);
	}
	void RemoveCharacter(Entity* character)
	{
		for (int i = 0; i < characters.size(); i++)
		{
			if (characters[i] == character)
			{
				for (int j = i + 1; j < characters.size(); j++)
				{
					characters[j - 1] = characters[j];
				}
				characters.pop_back();
				return;
			}
		}
	}

};