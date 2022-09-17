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

	if (maxX - minX >= (maxY - minY) * (float)((float)width / (float)height))
	{
		cameraEnd.x = minX - cameraOffset;
		cameraEnd.w = (maxX - minX) + cameraOffset * 2;
		cameraEnd.h = cameraEnd.w * (float)((float)height / (float)width);
		cameraEnd.y = minY - (cameraEnd.h - (maxY - minY)) / 2;
	}
	else
	{
		cameraEnd.y = minY - cameraOffset * (float)((float)height / (float)width);
		cameraEnd.h = (maxY - minY) + cameraOffset * 2 * (float)((float)height / (float)width);
		cameraEnd.w = cameraEnd.h * ((float)((float)width / (float)height));
		cameraEnd.x = minX - (cameraEnd.w - (maxX - minX)) / 2;
	}

	if (cameraEnd.w > width || cameraEnd.h > height)
	{
		cameraEnd.w = width;
		cameraEnd.h = height;
	}

	if (cameraEnd.w > stage->GetDeathZone()->w)
	{
		cameraEnd.w = stage->GetDeathZone()->w;
		cameraEnd.h = stage->GetDeathZone()->w * (float)((float)height / (float)width);
	}
	if (cameraEnd.h > stage->GetDeathZone()->h)
	{
		cameraEnd.h = stage->GetDeathZone()->h;
		cameraEnd.w = stage->GetDeathZone()->h * (float)((float)width / (float)height);
	}

	if (cameraEnd.x < 0)
	{
		cameraEnd.x = 0;
	}
	if (cameraEnd.x + cameraEnd.w > stage->GetDeathZone()->w)
	{
		cameraEnd.x = stage->GetDeathZone()->w - cameraEnd.w;
	}

	if (cameraEnd.y < 0)
	{
		cameraEnd.y = 0;
	}
	if (cameraEnd.y + cameraEnd.h > stage->GetDeathZone()->h)
	{
		cameraEnd.y = stage->GetDeathZone()->h - cameraEnd.h;
	}

	auxCam.x += (cameraEnd.x - auxCam.x) * 0.2f;
	auxCam.y += (cameraEnd.y - auxCam.y) * 0.2f;
	auxCam.w += (cameraEnd.w - auxCam.w) * 0.2f;
	auxCam.h = auxCam.w * (float)((float)height / (float)width);

	camera = auxCam;

	camera.x += camShake.getX();
	camera.y += camShake.getY();

	if (shakeDuration % 4 == 1)
		camShake = { camShake.getX() * -0.9f, camShake.getY() * -0.9f };

	if (shakeDuration > 0)
		shakeDuration--;
	else
	{
		camShake = { 0, 0 };
	}
	
}



FightManager::FightManager(SDLUtils * sdl) : sdl(sdl)
{
	int w = width, h = height;
	SDL_GetWindowSize(sdl->window(), &w, &h);

	width = w;
	height = h;

	listener = new MyListener();
	stage = new Stage(this, sdl, listener, step);
	

	sizeDiff = width / sdl->width() < height / sdl->height() ? 
		width / sdl->width() : height / sdl->height();

	cameraOffset *= sizeDiff;

	
	for (auto i = 0u; i < 1000; i++) {
		string s = "nes" + to_string(i);
		sdl->fonts().emplace(s, Font("resources/fonts/NES-Chimera.ttf", i));
	}

	setState(new MenuState(this));
	while (!exit_) {

		Uint32 startTime = sdl->currRealTime();

		SDL_Event e;
		ih.clearState();
		while (SDL_PollEvent(&e))
		{
			ih.update(e);
			if (e.type == SDL_QUIT)
			{
				userExit();
			}
		}

		getState()->update();

		if (!exit_) {
			getState()->draw();
		}

		double frameTime = sdl->currRealTime() - startTime;

		if (frameTime < (step * 1000))
		{
			SDL_Delay((step * 1000));
		}
	}
	// En ExitState se borran el state y el exitState, el savedState se borra en esta destructora
	delete getState();

}

FightManager::~FightManager()
{
	delete getSavedState();
	delete listener;
	for (auto e : entities)
		delete e;
	entities.clear();
	delete stage;
}

void FightManager::Update()
{

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
		MoveCamera();

		//update
		for (auto i = 0u; i < entities.size(); i++)
		{
			entities[i]->update();
		}

		//Checkeamos todas las colisiones
		for (auto i = 0u; i < entities.size(); i++)
		{
			entities[i]->CheckHits();
		}
	}

	//Dibuja las entidades
	for (int i = entities.size() - 1; i >= 0; i--)
	{
		entities[i]->draw(&camera);
	}

	//Elimina entidades a borrar;
	for (auto i = 0u; i < entities.size(); i++)
	{
		if (entities[i]->ToDelete())
		{
 			if (!entities[i]->isCharacter())
			{
				RemoveEntity(entities[i]);
				if (entities.size() > 1)
					i--;
			}
			else
			{
				if (characters.size() > 1)
				{
					Character* aux = static_cast<Character*>(entities[i]);
					RemoveCharacter(aux);
					if (entities.size() > 1)
						i--;
				}
			}
		}
	}

	// present new frame
	sdl->presentRenderer();
	
	if (endGameTimer + 1000 < SDL_GetTicks() && endGame) {

		getState()->next();
	}

}

void FightManager::LoadStage(std::string file)
{
	stage->LoadJsonStage(file, width, height);
}

void FightManager::InitCamera()
{

	if (stage->GetDeathZone()->w < stage->GetDeathZone()->h * (width / height))
	{
		camera = {
			0,
			(stage->GetDeathZone()->h / 2) - (stage->GetDeathZone()->w * (height / width) / 2),
			stage->GetDeathZone()->w,
			stage->GetDeathZone()->w * (height / width) };
		auxCam = camera;
	}
	else
	{
		camera = {
			(stage->GetDeathZone()->w / 2) - (stage->GetDeathZone()->h * (width / height) / 2),
			0,
			stage->GetDeathZone()->h * (width / height),
			stage->GetDeathZone()->h };
		auxCam = camera;
	}
}

ushort FightManager::StartFight(std::vector<Character*> ent)
{

	InitCamera();
	
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
ushort FightManager::StartFight(std::vector<Character*> ateam1 , std::vector<Character*> ateam2)
{

	InitCamera();

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
						if (e == character)
						{
							belongsTeam1 = true; break;
						}
					}
					if (belongsTeam1) { // Si pertenece al team1
						vector<ushort>stats;
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
						vector<ushort>stats;
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

void FightManager::KillingBlow()
{
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

b2Vec2* FightManager::GetDeathZoneB2()
{
	return stage->GetDeathZoneB2();
}

b2World* FightManager::GetWorld()
{
	return stage->GetWorld();
}

void FightManager::addCharacterStats(Character* character)
{
	vector<ushort>stats;
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
	
	scount = 4;
	startticks = 0;

}
void FightManager::SetShake(const Vector2D& dir, uint16 duration)
{
	camShake = dir;
	shakeDuration = duration;
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
		string fontstring = "nes" + to_string((int)(120 * sizeDiff));
		auto& font = sdl->fonts().at(fontstring);
		count = new Texture(sdl->renderer(), s, font, build_sdlcolor(0x00F7FF00));
		x = (width / 2) - count->width() / 2;
		y = (height / 2) - count->height() / 2;
	}
	else
	{
		s = "FIGHT!";
		string fontstring = "nes" + to_string((int)(80 * sizeDiff));
		auto& font = sdl->fonts().at(fontstring);
		count = new Texture(sdl->renderer(), s, font, build_sdlcolor(0xFF000000));
		x = (width / 2) - count->width() / 2;
		y = (height / 2) - count->height() / 2;
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

ushort FightManager::GetActualWidth()
{
	return width;
}

ushort FightManager::GetActualHeight()
{
	return height;
}

double FightManager::GetScreenRatio()
{
	return stage->GetScreenRatio();
}

double FightManager::GetScreeAdjust()
{
	return sizeDiff;
}