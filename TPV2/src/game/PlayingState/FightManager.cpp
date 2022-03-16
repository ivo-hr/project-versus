#include "FightManager.h"
#include "../Entity.h"
#include "../Utils/MyListener.h"

FightManager::FightManager(SDLUtils* sdl, double screenAdjust) : world(b2World(b2Vec2(0.f, 20.f))), sdl(sdl)
{

	//Definimos un objeto (estatico)
	b2BodyDef groundDef;
	groundDef.position.Set(25.f, 20.f);
	groundDef.type = b2_staticBody;

	//Anadimos al mundo
	stage = world.CreateBody(&groundDef);;
	//Le damos forma...
	b2PolygonShape floor;
	float floorW = 25.f, floorH = 3.f;
	floor.SetAsBox(floorW / 2, floorH / 2);

	//..cuerpo
	b2FixtureDef fixt;
	fixt.shape = &floor;
	fixt.density = 10.0f;
	fixt.friction = 0.5f;
	fixt.filter.categoryBits = 2;
	fixt.filter.maskBits = 1; // Colisiona con los personajes (tienen este categoryBits en Entity)

	stage->CreateFixture(&fixt);

	b2ToSDL = (sdl->width() * screenAdjust) / 50.f;

	//--------------------------

	//Creo las cajas que representaran a los objetos
	stageRect = GetSDLCoors(stage, floorW, floorH);
	platformRect = GetSDLCoors(platform, platW, platH);

	listener = new MyListener();
	world.SetContactListener(listener);

	deathZone = { 0, 0, (int)(sdl->width() * screenAdjust), (int)(sdl->height() * screenAdjust) };
}

FightManager::~FightManager()
{
}

int FightManager::StartFight(Entity* p1, Entity* p2)
{
	AddEntity(p1);
	AddEntity(p2);

	p1->SetOponents(entities);
	p2->SetOponents(entities);

	listener->AddCharacter(p1);
	listener->AddCharacter(p2);

	bool exit_ = false;
	while (!exit_ && !fightEnded) {

		Uint32 startTime = sdl->currRealTime();


		ih.refresh();		//QUE WEA

		if (ih.isKeyDown(SDLK_ESCAPE))
			exit_ = true;

		//Esto llama al mundo para que simule lo que pasa en el tiempo que se le pase (en este caso 1000.f/60.f (un frame a 60 fps))
		float step = 1.f / 60.f;
		world.Step(step, 1, 1);

		// clear screen
		sdl->clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

		//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d

		//Dibujamos las cajas
		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &stageRect);
		SDL_RenderDrawRect(sdl->renderer(), &platformRect);
		SDL_RenderDrawRect(sdl->renderer(), &deathZone);

		for (Entity* ent : entities)
		{
			ent->update();
		}
		for (Entity* ent : entities)
		{
			ent->draw();
		}

		while (addedDelay > 0)
		{

			Uint32 startTime = sdl->currRealTime();
			addedDelay--;

			for (Entity* ent : entities)
			{
				ent->draw();
			}
			// present new frame
			sdl->presentRenderer();

			double frameTime = sdl->currRealTime() - startTime;

			if (frameTime < (step * 1000))
			{
				SDL_Delay((step * 1000));
			}
		}

		// present new frame
		sdl->presentRenderer();

		double frameTime = sdl->currRealTime() - startTime;

		if (frameTime < (step * 1000))
		{
			SDL_Delay((step * 1000));
		}

		addedDelay = 0;
	}

	return 1;
}

void FightManager::AddEntity(Entity* ent)
{
	entities.push_back(ent);
}

bool FightManager::RemoveEntity(Entity* ent)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] == ent)
		{
			for (int j = i + 1; j < entities.size(); j++)
			{
				entities[j - 1] = entities[j];
			}
			entities.pop_back();
			delete ent;
		}
	}
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->SetOponents(entities);
	}
	return false;
}

void FightManager::HitLag(int frames)
{
	if (addedDelay < frames)
		addedDelay = frames;
}

void FightManager::FighterLost(Entity* loser)
{
	RemoveEntity(loser);
	numPlayers--;

	if (numPlayers == 1)
	{
		fightEnded = true;
		winner = entities[0];
	}
}

std::vector<Entity*> FightManager::GetOponents(Entity* current)
{
	std::vector<Entity*> a;

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] != current)
		{
			a.push_back(entities[i]);
		}
	}
	return a;
}

SDL_Rect FightManager::GetSDLCoors(b2Body* body, float width, float height)
{
	return { (int)((body->GetPosition().x * b2ToSDL) - (width * b2ToSDL) / 2.f),
		(int)((body->GetPosition().y * b2ToSDL) - (height * b2ToSDL) / 2.f),
		(int)(width * b2ToSDL),
		(int)(height * b2ToSDL) };
}

SDL_Rect FightManager::GetSDLCoors(float x, float y, float width, float height)
{
	return { (int)((x * b2ToSDL) - (width * b2ToSDL) / 2.f),
		(int)((y * b2ToSDL) - (height * b2ToSDL) / 2.f),
		(int)(width * b2ToSDL),
		(int)(height * b2ToSDL) };
}

int FightManager::b2ToSDLX(b2Body* body, float width)
{
	return (int)(body->GetPosition().x * b2ToSDL - width * b2ToSDL / 2);
}

int FightManager::b2ToSDLY(b2Body* body, float height)
{
	return (int)(body->GetPosition().y * b2ToSDL - height * b2ToSDL / 2);
}
