#include "FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../PlayingState/Stage.h"

void FightManager::MoveCamera()
{
	SDL_Rect cameraEnd;
	int maxX = INT32_MIN, minX = INT32_MAX;
	int maxY = INT32_MIN, minY = INT32_MAX;

	for (Entity* c : characters)
	{
		Vector2D pos = c->GetCenterSDL();

		if (pos.getX() > maxX)
		{
			maxX = pos.getX();
		}
		if (pos.getX() < minX)
		{
			minX = pos.getX();
		}

		if (pos.getY() > maxY)
		{
			maxY = pos.getY();
		}
		if (pos.getY() < minY)
		{
			minY = pos.getY();
		}
	}

	if (maxX - minX >= (maxY - minY) * ((float)(sdl->width()) / (float)(sdl->height())))
	{
		cameraEnd.x = minX - cameraOffset;
		cameraEnd.w = (maxX - minX) + cameraOffset * 2;
		cameraEnd.h = cameraEnd.w * ((float)((float)(sdl->height()) / (float)(sdl->width())));
		cameraEnd.y = minY - (cameraEnd.h - (maxY - minY)) / 2;
	}
	else
	{
		cameraEnd.y = minY - cameraOffset * (float)((float)(sdl->height()) / (float)(sdl->width()));
		cameraEnd.h = (maxY - minY) + cameraOffset * 2 * (float)((float)(sdl->height()) / (float)(sdl->width()));
		cameraEnd.w = cameraEnd.h * ((float)((float)(sdl->width()) / (float)(sdl->height())));
		cameraEnd.x = minX - (cameraEnd.w - (maxX - minX)) / 2;
	}

	camera.x += (cameraEnd.x - camera.x) * 0.2f;
	camera.y += (cameraEnd.y - camera.y) * 0.2f;
	camera.w += (cameraEnd.w - camera.w) * 0.2f;
	camera.h = camera.w * ((float)(sdl->height()) / (float)(sdl->width()));

	//camera.x = cameraEnd.x;
	//camera.y = cameraEnd.y;
	//camera.w = cameraEnd.w;
	//camera.h = cameraEnd.h;

}

FightManager::FightManager(SDLUtils * sdl, double screenAdjust) :  sdl(sdl)
{
	listener = new MyListener();
	stage = new Stage(sdl, listener, screenAdjust, step, "resources/config/stage1.json");

	camera = { 100, 100, (int)(sdl->width() * screenAdjust), (int)(sdl->height() * screenAdjust) };

	this->screenAdjust = screenAdjust;

	setState(new MenuState(this));
	while (!exit_) {
		ih.refresh();
		if (ih.isKeyDown(SDLK_ESCAPE))
			exit_ = true;
		getState()->update();
		getState()->draw();
	}
}

FightManager::~FightManager()
{
}

void FightManager::Update()
{

	Uint32 startTime = sdl->currRealTime();

	if (ih.isKeyDown(SDLK_ESCAPE))
		exit_ = true;
	if (ih.isKeyDown(SDLK_p) && ih.keyDownEvent()) {
		if (getSavedState() == nullptr) {
			//pause
			std::cout << "pause" << std::endl;
			saveState(getState());
			setState(new PauseState(this));
				return;
		}
	}

	MoveCamera();

	stage->Update(&camera);

	stage->GetWorld()->Step(step, 1, 1);

	for (Particle* part : particulas)
	{
		part->draw(&camera);
		part->update();
	}
	for (auto i = 0u; i < entities.size(); i++)
	{
		entities[i]->update();
	}
	for (Entity* ent : entities)
	{
		ent->CheckHits();
	}
	for (int i = entities.size() - 1; i >= 0; i--)
	{
		entities[i]->draw(&camera);
	}

	while (addedDelay > 0)
	{

		Uint32 startTime = sdl->currRealTime();
		addedDelay--;

		if (addedDelay % 3 == 0)
		{
			hitLagCam.x -= camera.x;
			hitLagCam.x *= -0.9f;
			hitLagCam.x += camera.x;

			hitLagCam.y -= camera.y;
			hitLagCam.y *= -0.9f;
			hitLagCam.y += camera.y;
		}

		stage->Update(&hitLagCam);

		for (Particle* part : particulas)
		{
			part->draw(&hitLagCam);
			part->update();
		}
		for (Entity* ent : entities)
		{
			ent->updateParticles();
		}
		for (int i = entities.size() - 1; i >= 0; i--)
		{
			entities[i]->draw(&hitLagCam);
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
}

int FightManager::StartFight(std::vector<Entity*> ent)
{
	entities = ent;
	characters = ent;

	for (auto e : entities) {
		e->SetOponents(entities);
		listener->AddCharacter(e);
	}
	sdl->musics().at("running_grass").play();
	Music::setMusicVolume(1);

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
		}
	}
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->DeleteOponent(ent);
	}
	delete ent;
	return false;
}

void FightManager::MoveToFront(Entity* ent)
{
	for (auto i = 0u; i < entities.size(); i++)
	{
		if (entities[i] == ent)
		{
			for (int j = i; j > 0; j--)
			{
				entities[j] = entities[j - 1];
			}
			entities[0] = ent;
			return;
		}
	}
}

void FightManager::AddParticle(Particle* par)
{
	particulas.push_back(par);
}

bool FightManager::RemoveParticle(Particle* par)
{
	for (int i = 0; i < particulas.size(); i++)
	{
		if (particulas[i] == par)
		{
			for (int j = i + 1; j < particulas.size(); j++)
			{
				particulas[j - 1] = particulas[j];
			}
			particulas.pop_back();
		}
	}
	delete par;
	return false;
}

void FightManager::HitLag(int frames)
{
	if (addedDelay < frames)
		addedDelay = frames;

	hitLagCam = camera;

	hitLagCam.x += (addedDelay * 0.4f) * (camera.w * 0.005f);

	hitLagCam.y += addedDelay * 0.2f * (camera.w * 0.005f);
}

void FightManager::KillingBlow(Vector2D dead)
{
	HitLag(40);
	AddParticle(new Particle(
		&dead,
		1, "killVfx", this));
	AddParticle(new Particle(
		&dead,
		1, "killHit", this));
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

SDL_Rect* FightManager::GetDeathZone()
{
	return stage->GetDeathZone();
}

b2World* FightManager::GetWorld()
{
	return stage->GetWorld();
}

SDL_Rect FightManager::GetSDLCoors(b2Body* body, float width, float height)
{
	return stage->GetSDLCoors(body, width, height);
}

SDL_Rect FightManager::GetSDLCoors(float x, float y, float width, float height)
{
	return stage->GetSDLCoors(x, y, width, height);
}

int FightManager::b2ToSDLX(b2Body* body, float width)
{
	return stage->b2ToSDLX(body, width);
}

int FightManager::b2ToSDLY(b2Body* body, float height)
{
	return stage->b2ToSDLY(body, height);
}

int FightManager::ToSDL(float x)
{
	return x * stage->getb2ToSDL();
}

int FightManager::GetActualWidth()
{
	return sdl->width() * screenAdjust;
}

int FightManager::GetActualHeight()
{
	return sdl->height() * screenAdjust;
}

double FightManager::GetScreenRatio()
{
	return stage->GetScreenRatio();
}

double FightManager::GetScreeAdjust()
{
	return screenAdjust;
}
