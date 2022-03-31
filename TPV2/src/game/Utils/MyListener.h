#include <string>
#include "../../sdlutils/InputHandler.h"
#include<box2d.h>
#include"../Entity.h"
#include<vector>

class MyListener : public b2ContactListener
{
private:
	std::vector<Entity*> characters;

public:
	void BeginContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		// Mira si está en contacto con el suelo
		if (one->GetType() == b2_staticBody && contact->IsEnabled())
		{
			for (int i = 0; i < characters.size(); i++)
			{
				if (two == characters[i]->GetBody())
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
		// Mira si deja de contactar con el suelo
		if (one->GetType() == b2_staticBody)
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
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		// Mira si quiere subir a la plataforma (atravesándola)
		if (one->GetFixtureList()->GetFilterData().categoryBits == 4 && (two->GetPosition().y > one->GetPosition().y))
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

};