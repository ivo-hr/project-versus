#include "FightManager.h"
#include "../Entity.h"
#include "../Character.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../PlayingState/Stage.h"

void FightManager::MoveCamera()
{
	SDL_Rect cameraEnd;
	int maxX = INT32_MIN, minX = INT32_MAX;
	int maxY = INT32_MIN, minY = INT32_MAX;

	for (Character* c : characters)
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

	if (cameraEnd.w > sdl->width() * screenAdjust)
	{
		cameraEnd.w = sdl->width() * screenAdjust;
		cameraEnd.h = sdl->height() * screenAdjust;
	}
	if (cameraEnd.x < 0)
	{
		cameraEnd.x = 0;
	}
	else if (cameraEnd.x + cameraEnd.w > sdl->width() * screenAdjust)
	{
		cameraEnd.x = sdl->width() * screenAdjust - cameraEnd.w;
	}

	if (cameraEnd.y < 0)
	{
		cameraEnd.y = 0;
	}
	else if (cameraEnd.y + cameraEnd.h > sdl->height() * screenAdjust)
	{
		cameraEnd.y = sdl->height() * screenAdjust - cameraEnd.h;
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
	stage = new Stage(sdl, listener, step);

	camera = { 0, 0, (int)(sdl->width() * screenAdjust), (int)(sdl->height() * screenAdjust)};

	this->screenAdjust = screenAdjust;

	cameraOffset *= screenAdjust;

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
			saveState(getState());
			setState(new ConfigurationState(this));
			return;
		}
		else
		{
			State* tmp = getState();
			State* saved = getSavedState();
			setState(saved);
			saveState(tmp);
		}
	}

	MoveCamera();

	stage->Update(&camera);

	stage->GetWorld()->Step(step, 1, 1);

	for (auto i = 0u; i < entities.size(); i++)
	{
		entities[i]->update();
	}
	for (int i = entities.size() - 1; i >= 0; i--)
	{
		entities[i]->draw(&camera);
	}
	for (Entity* ent : entities)
	{
		ent->CheckHits();
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

		for (int i = entities.size() - 1; i >= 0; i--)
		{
			entities[i]->updateParticles();
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

void FightManager::LoadStage(std::string file)
{
	stage->LoadJsonStage(file, screenAdjust);
}

int FightManager::StartFight(std::vector<Character*> ent)
{

	for (Character* a : ent)
	{
		entities.push_back(a);
		characters.push_back(a);
	}
	//characters = ent;

	for (auto i = 0u; i < characters.size(); i++) {
		numPlayers++;
		characters[i]->SetOponents(entities);
		listener->AddCharacter(entities[i]);
		characters[i]->SetSpawn(stage->GetPlayerSpawns(i), stage->GetPlayerDir(i)); 
		characters[i]->SetPNumber(i);
	}
	sdl->musics().at("cube").play();
	//Music::setMusicVolume(1);

	return 1;
}
int FightManager::StartFight(std::vector<Character*> team1 , std::vector<Character*> team2)
{
	std::vector<Entity*> aux1;
	std::vector<Entity*> aux2;
	for (Character* a : team1)
	{
		entities.push_back(a);
		characters.push_back(a);
		aux1.push_back(a);
	}

	for (Character* a : team2)
	{
		entities.push_back(a);
		characters.push_back(a);
		aux2.push_back(a);
	}
	//characters = team1;


	for (auto i = 0u; i < team1.size(); i++) {
		numPlayers++;
		characters[i]->SetOponents(aux2);
		listener->AddCharacter(characters[i]);
		characters[i]->SetSpawn(stage->GetPlayerSpawns(i), stage->GetPlayerDir(i));
		characters[i]->SetPNumber(0);
	}
	for (auto i = team1.size(); i < team2.size()+team1.size() ; i++) {
		numPlayers++;
		characters[i]->SetOponents(aux1);
		listener->AddCharacter(characters[i]);
		characters[i]->SetSpawn(stage->GetPlayerSpawns(i), stage->GetPlayerDir(i));
		characters[i]->SetPNumber(1);
	}
	sdl->musics().at("cube").play();
	//Music::setMusicVolume(1);

	return 1;
}
int FightManager::StartFight(std::vector<Character*> team1, std::vector<Character*> team2, std::vector<Character*> team3)
{
	for (Character* a : team1)
	{
		entities.push_back(a);
		characters.push_back(a);
	}

	for (Character* a : team2)
	{
		entities.push_back(a);
		characters.push_back(a);
	}
	for (Character* a : team3)
	{
		entities.push_back(a);
		characters.push_back(a);
	}
	for (auto i = 0u; i < characters.size(); i++) {
		numPlayers++;
		characters[i]->SetOponents(entities);
		listener->AddCharacter(characters[i]);
		characters[i]->SetSpawn(stage->GetPlayerSpawns(i), stage->GetPlayerDir(i));
		characters[i]->SetPNumber(i);
	}
	sdl->musics().at("cube").play();
	//Music::setMusicVolume(1);
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

bool FightManager::RemoveCharacter(Character* character)
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
		}
	}
	listener->RemoveCharacter(character);
	RemoveEntity(character);
	if (characters.size() == 1) {
		winnerInput = characters[0]->getInput();
		winnersTextures.push_back(characters[0]->getPortrait());
		entities.clear();
		characters.clear();
		getState()->next();
	}
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

void FightManager::AddOponnent(Entity* ent, Entity* ignore)
{
	for (auto i = 0u; i < entities.size(); i++)
	{
		if (entities[i] != ent && entities[i] != ignore)
		{
			entities[i]->AddOponent(ent);
		}
	}
}

void FightManager::HitLag(int frames)
{
	if (addedDelay < frames)
		addedDelay = frames;

	hitLagCam = camera;

	hitLagCam.x += (addedDelay * 0.4f) * (camera.w * 0.005f);

	hitLagCam.y += addedDelay * 0.2f * (camera.w * 0.005f);
}

void FightManager::KillingBlow()
{
	HitLag(40);

	sdl->soundEffects().at("hitKill").play();
}

void FightManager::FighterLost(Character* loser)
{
	RemoveCharacter(loser);
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