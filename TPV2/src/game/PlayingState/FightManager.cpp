#include "FightManager.h"
#include "../Entity.h"
#include "../Character.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../PlayingState/Stage.h"
#include "../../utils/CheckML.h"

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

	
	for (auto i = 0u; i < 1000; i++) {
		string s = "nes" + to_string(i);
		sdl->fonts().emplace(s, Font("resources/fonts/NES-Chimera.ttf", i));
	}

	setState(new MenuState(this));
	while (!exit_) {
		ih.refresh();
		getState()->update();
		if (!exit_) {
			getState()->draw();
		}
	}
	if(getSavedState()!=nullptr)
	delete getState();
}

FightManager::~FightManager()
{
	delete stage;
	delete listener;
	for (auto e : entities)
		delete e;
	entities.clear();
	delete getSavedState();
}

void FightManager::Update()
{

	Uint32 startTime = sdl->currRealTime();
	if (ih.isKeyDown(SDLK_ESCAPE) && ih.keyDownEvent()) {
		if (getSavedState() == nullptr) {
			//pause
			saveState(getState());
			setState(new ConfigurationState(this,characters[0]->getInput()));
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
	
	for (Character* c : characters)
	{
		c->drawHUD( numPlayers);
	}
	if (scount > -1) {
		startCount();
	}
	else
	{
		for (auto i = 0u; i < entities.size(); i++)
		{
			entities[i]->update();
		}
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

		for (Character* c : characters)
		{
			c->drawHUD( numPlayers);
		}

		for (int i = entities.size() - 1; i >= 0; i--)
		{
			entities[i]->updateParticles();
			entities[i]->draw(&hitLagCam);
		}

		HideOutOfBounds();

		// present new frame
		sdl->presentRenderer();

		double frameTime = sdl->currRealTime() - startTime;

		if (frameTime < (step * 1000))
		{
			SDL_Delay((step * 1000));
		}
	}
	
	HideOutOfBounds();

	// present new frame
	sdl->presentRenderer();

	double frameTime = sdl->currRealTime() - startTime;

	if (frameTime < (step * 1000))
	{
		SDL_Delay((step * 1000));
	}
	
	if (endGameTimer + 1000 < SDL_GetTicks() && endGame) {

		getState()->next();
	}

}

void FightManager::HideOutOfBounds()
{
	int w, h;
	SDL_GetWindowSize(sdl->window(), &w, &h);

	if (sdl->width() * screenAdjust < w)
	{
		SDL_Rect camOOBX = { sdl->width() * screenAdjust, 0, w - (sdl->width() * screenAdjust), h };

		SDL_SetRenderDrawColor(sdl->renderer(), 0, 0, 0, 255);
		SDL_RenderFillRect(sdl->renderer(), &camOOBX);
	}
	else if (sdl->height() * screenAdjust < h)
	{
		SDL_Rect camOOBY = { 0, sdl->height() * screenAdjust, w, h - (sdl->height() * screenAdjust) };

		SDL_SetRenderDrawColor(sdl->renderer(), 0, 0, 0, 255);
		SDL_RenderFillRect(sdl->renderer(), &camOOBY);
	}
}

void FightManager::LoadStage(std::string file)
{
	stage->LoadJsonStage(file, screenAdjust);
}

int FightManager::StartFight(std::vector<Character*> ent)
{
	
	//onNewGame();
	teammode = false;
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

	auto mus = RandomNumberGenerator().nextInt(0, 3);

	switch (mus) {
		case 0:
			sdl->musics().at("cube").play();
			break;
		case 1:
			sdl->musics().at("prep").play();
			break;
		case 2:
			sdl->musics().at("decis").play();
			break;

	}
	numPlayers = characters.size();
	
	scount = 4;
	startticks = 0;
	return 1;
}
int FightManager::StartFight(std::vector<Character*> ateam1 , std::vector<Character*> ateam2)
{
	//onNewGame();
	teammode = true;
	std::vector<Entity*> aux1;
	std::vector<Entity*> aux2;
	for (Character* a : ateam1)
	{
		team1.push_back(a);
		entities.push_back(a);
		characters.push_back(a);
		aux1.push_back(a);
	}

	for (Character* a : ateam2)
	{
		team2.push_back(a);
		entities.push_back(a);
		characters.push_back(a);
		aux2.push_back(a);
	}
	//characters = team1;


	for (auto i = 0u; i < ateam1.size(); i++) {
		//numPlayers++;
		characters[i]->SetOponents(aux2);
		listener->AddCharacter(characters[i]);
		characters[i]->SetSpawn(stage->GetPlayerSpawns(i), stage->GetPlayerDir(i));
		characters[i]->SetPNumber(0);
	}
	for (auto i = ateam1.size(); i < ateam2.size()+ateam1.size() ; i++) {
		//numPlayers++;
		characters[i]->SetOponents(aux1);
		listener->AddCharacter(characters[i]);
		characters[i]->SetSpawn(stage->GetPlayerSpawns(i), stage->GetPlayerDir(i));
		characters[i]->SetPNumber(1);
	}
	sdl->musics().at("cube").play();
	//Music::setMusicVolume(1);
	numPlayers = characters.size();
	scount = 4;
	startticks = 0;
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
	if (!endGame) {
		for (int i = 0; i < characters.size(); i++)
		{
			if (characters[i] == character)
			{
				if (!teammode) {
					addCharacterStats(characters[i]);
					deadTextures.push_back(characters[i]->getPortrait());
				}
				else {
					bool belongsTeam1 = false;
					for (auto e : team1) {
						if (e == character)belongsTeam1 = true;
					}
					if (belongsTeam1) { // Si pertenece al team1
						vector<int>stats;
						stats.push_back(characters[i]->getDeaths());
						stats.push_back(characters[i]->getDamageTaken());
						stats.push_back(characters[i]->getKills());
						team1DeadStats.push_back(stats);
						team1DeadTextures.push_back(characters[i]->getPortrait());
						for (int x = i + 1; x < team1.size(); x++)
						{
							team1[x - 1] = team1[x];
						}
						team1.pop_back();
					}
					else // Si pertenece al team2
					{
						vector<int>stats;
						stats.push_back(characters[i]->getDeaths());
						stats.push_back(characters[i]->getDamageTaken());
						stats.push_back(characters[i]->getKills());
						team2DeadStats.push_back(stats);
						team2DeadTextures.push_back(characters[i]->getPortrait());
						for (int y = (i % 2) + 1; y < team2.size(); y++)
						{
							team2[y - 1] = team2[y];
						}
						team2.pop_back();
					}
				}
				for (int j = i + 1; j < characters.size(); j++)
				{
					characters[j - 1] = characters[j];
				}
				characters.pop_back();
			}
		}
		listener->RemoveCharacter(character);
		RemoveEntity(character);
		if (!teammode) {
			if (characters.size() == 1 && !endGame) {
				addCharacterStats(characters[0]);
				winnerInput = characters[0]->getInput();
				deadTextures.push_back(characters[0]->getPortrait());
				endGameTimer = SDL_GetTicks();
				endGame = true;
			}
		}
		else
		{
			if (team1.size() == 0 && !endGame) { // Si team1 pierde
				for (auto e : team1DeadStats)gameStats.push_back(e);
				for (auto e : team1DeadTextures)deadTextures.push_back(e);
				for (auto e : team2DeadStats)gameStats.push_back(e);
				for (auto e : team2DeadTextures)deadTextures.push_back(e);
				for (auto e : characters)addCharacterStats(e);
				for (auto e : characters)deadTextures.push_back(e->getPortrait());
				winnerInput = characters[0]->getInput();
				endGameTimer = SDL_GetTicks();
				endGame = true;
			}
			else if (team2.size() == 0 && !endGame) { // Si team2 pierde
				for (auto e : team2DeadStats)gameStats.push_back(e);
				for (auto e : team2DeadTextures)deadTextures.push_back(e);
				for (auto e : team1DeadStats)gameStats.push_back(e);
				for (auto e : team1DeadTextures)deadTextures.push_back(e);
				for (auto e : characters)addCharacterStats(e);
				for (auto e : characters)deadTextures.push_back(e->getPortrait());
				winnerInput = characters[0]->getInput();
				endGameTimer = SDL_GetTicks();
				endGame = true;
			}
		}
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


std::vector<Entity*> FightManager::GetEntities(Entity* current)
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

void FightManager::addCharacterStats(Character* character)
{
	vector<int>stats;
	stats.push_back(character->getDeaths());
	stats.push_back(character->getDamageTaken());
	stats.push_back(character->getKills());
	gameStats.push_back(stats);
}

void FightManager::onNewGame()
{
	
	for (auto e : entities) {
		listener->RemoveCharacter(e);
		delete e;
	}
	entities.clear();
	characters.clear();
	team1.clear();
	team2.clear();
	stage->UnLoadStage();
	deadTextures.clear();
	team1DeadStats.clear();
	team1DeadTextures.clear();
	team2DeadStats.clear();
	team2DeadTextures.clear();
	endGame = false;
	endGameTimer = 0;
	gameStats.clear();
	int scount = 4;
	int startticks = 0;

}
void FightManager::startCount()
{
	Texture* count;
	string s;
	int x = 0;
	int y = 0;
	if (startticks + 1000 < SDL_GetTicks()) {
		startticks = SDL_GetTicks();
		scount--;
	}
	if (scount > 0)
	{
		s = to_string(scount);
		string fontstring = "nes" + to_string((int)(120 * screenAdjust));
		auto& font = sdl->fonts().at(fontstring);
		count = new Texture(sdl->renderer(), s, font, build_sdlcolor(0x00F7FF00));
		x = 200 * screenAdjust;
		y = 100 * screenAdjust;
	}
	else
	{
		s = "FIGHT!";
		string fontstring = "nes" + to_string((int)(80 * screenAdjust));
		auto& font = sdl->fonts().at(fontstring);
		count = new Texture(sdl->renderer(), s, font, build_sdlcolor(0xFF000000));
		x = 20 * screenAdjust;
		y = 100 * screenAdjust;
	}
	count->render(x,y);
	delete count;
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