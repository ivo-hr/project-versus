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

	for (auto c : camFollow)
	{
		if (c->IsAlive())
		{
			Vector2D pos = c->GetCenterSDL();

			if (pos.getX() > maxX)
			{
				maxX = (int)pos.getX();
			}
			if (pos.getX() < minX)
			{
				minX = (int)pos.getX();
			}

			if (pos.getY() > maxY)
			{
				maxY = (int)pos.getY();
			}
			if (pos.getY() < minY)
			{
				minY = (int)pos.getY();
			}
		}
	}

	float whr = (float)width / (float)height;
	float hwr = (float)height / (float)width;

	if (maxX - minX >= (maxY - minY) * whr)
	{
		cameraEnd.x = minX - cameraOffset;
		cameraEnd.w = (maxX - minX) + cameraOffset * 2;
		cameraEnd.h = (int)(cameraEnd.w * hwr);
		cameraEnd.y = minY - (cameraEnd.h - (maxY - minY)) / 2;
	}
	else
	{
		cameraEnd.y = minY - (int)((float)cameraOffset * hwr);
		cameraEnd.h = (maxY - minY) + (int)((int)cameraOffset * 2 * hwr);
		cameraEnd.w = (int)(cameraEnd.h * whr);
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
		cameraEnd.h = (int)(stage->GetDeathZone()->w * hwr);
	}
	if (cameraEnd.h > stage->GetDeathZone()->h)
	{
		cameraEnd.h = stage->GetDeathZone()->h;
		cameraEnd.w = (int)(stage->GetDeathZone()->h * whr);
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

	int diffX = (int)((cameraEnd.x - auxCam.x) * 0.2f);
	int diffY = (int)((cameraEnd.y - auxCam.y) * 0.2f);
	int diffW = (int)((cameraEnd.w - auxCam.w) * 0.2f);

	auxCam.x += diffX;
	auxCam.y += diffY;
	auxCam.w += diffW;
	auxCam.h = (int)(auxCam.w * hwr);

	camera = auxCam;

	camera.x += (int)nearbyint(camShake.getX());
	camera.y += (int)nearbyint(camShake.getY());

	if (shakeDuration % 2 == 1)
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

	sizeDiff = width / (float)sdl->width() < height / (float)sdl->height() ?
		(float)width / (float)sdl->width() : (float)height / (float)sdl->height();

	cameraOffset = (ushort)((float)cameraOffset * sizeDiff);

	entityMatrix = vector<vector<Entity*>>();
	
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
			SDL_Delay((Uint32)((step * 1000) - frameTime));
		}
		else
		{
			cout << "----------" << endl;
			cout << frameTime << "/" << (step * 1000) << endl;
			for (vector<Entity*> a : entityMatrix)
			{
				for (Entity* b : a)
				{
					cout << b->GetName() << "  ";
				}
				cout << endl;
			}
			cout << endl;
		}
	}
	// En ExitState se borran el state y el exitState, el savedState se borra en esta destructora
	delete getState();

}

FightManager::~FightManager()
{
	if (getSavedState())
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

	stage->GetWorld()->Step(step, 1, 1);

	if (scount > -1) {

		stage->Update(&camera);
		startCount();
	}
	else
	{
		MoveCamera();

		stage->Update(&camera);

		while (!toAdd.empty())
		{
			AddToGame(toAdd.front());
			toAdd.pop();
		}

		//update
		for (Entity* ent : entities)
		{
			ent->update();
		}

		//Checkeamos todas las colisiones
		for (Entity* ent : entities)
		{
			if (ent->IsAlive())
				ent->CheckHits();
		}
	}

	auto it = entities.end();
	//Dibuja las entidades
	while (it != entities.begin())
	{
		it--;
		if ((*it)->IsAlive())
			(*it)->draw(&camera);
	}

	//Elimina entidades a borrar;
	while (it != entities.end())
	{
		if ((*it)->ToDelete())
		{
			auto a = RemoveEntity(it);
			it = a;
		}
		if (it != entities.end())
			++it;
	}

	for (Character* c : characters)
	{
		c->drawHUD(numPlayers);
	}

	if (ih.isKeyDown(SDLK_1) && ih.keyDownEvent()) {
		TakeScreenShot();
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 0, 0, 0);
		SDL_Rect a = { 0, 0, width, height };
		SDL_RenderFillRect(sdl->renderer(), &a);
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
		//Init vectors
		entities.push_back(a);
		characters.push_back(a);
		camFollow.push_back(a);
		a->AddTag(Tags::IsCharacter);
		a->AddTag(Tags::CameraFollow);
	}
	//characters = ent;

	for (auto i = 0u; i < characters.size(); i++) {
		numPlayers++;
		listener->AddCharacter(entities[i]);
		characters[i]->SetPNumber(i);
		characters[i]->SetSpawn(stage->GetPlayerSpawns(i), stage->GetPlayerDir(i));
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
	numPlayers = (short)characters.size();

	InitMatrix();
	
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
		entities.push_back(a);
		characters.push_back(a);
		team1.push_back(&characters[characters.size() - 1]);
		aux1.push_back(a);
		camFollow.push_back(a);
		a->AddTag(Tags::IsCharacter);
		a->AddTag(Tags::CameraFollow);
	}

	for (Character* a : ateam2)
	{
		entities.push_back(a);
		characters.push_back(a);
		team2.push_back(&characters[characters.size() - 1]);
		aux2.push_back(a);
		camFollow.push_back(a);
		a->AddTag(Tags::IsCharacter);
		a->AddTag(Tags::CameraFollow);
	}
	//characters = team1;


	for (auto i = 0u; i < ateam1.size(); i++) {
		//numPlayers++;
		listener->AddCharacter(characters[i]);
		characters[i]->SetPNumber(0);
		characters[i]->SetSpawn(stage->GetPlayerSpawns(i), stage->GetPlayerDir(i));
	}
	for (auto i = ateam1.size(); i < ateam2.size()+ateam1.size() ; i++) {
		//numPlayers++;
		listener->AddCharacter(characters[i]);
		characters[i]->SetPNumber(1);
		characters[i]->SetSpawn(stage->GetPlayerSpawns((int)i), stage->GetPlayerDir((int)i));
	}
	sdl->musics().at("cube").play();
	//Music::setMusicVolume(1);
	numPlayers = (short)characters.size();

	InitMatrix();

	scount = 4;
	startticks = 0;
	return 1;
}

void FightManager::InitMatrix()
{

	if (!teammode)
	{
		entityMatrix = vector<vector<Entity*>>(entities.size() + 1);
		int j = 0;
		for (int i = 1; i < entities.size() + 1; i++)
		{
			entityMatrix[i].push_back(entities[j]);
			entities[j]->SetLayer(i);
			entities[j]->SetPlaceInLayer((ushort)entityMatrix[i].size());
			j++;
		}
	}
	else
	{
		if (team1.size() > 0 && team2.size() > 0)
		{
			entityMatrix = vector<vector<Entity*>>(3);
			for (int i = 0; i < team1.size(); i++)
			{
				entityMatrix[1].push_back(*team1[i]);
				(*team1[i])->SetLayer(1);
				entities[i]->SetPlaceInLayer((ushort)entityMatrix[1].size());
			}
			for (int i = 0; i < team2.size(); i++)
			{
				entityMatrix[2].push_back(*team2[i]);
				(*team2[i])->SetLayer(2);
				entities[i]->SetPlaceInLayer((ushort)entityMatrix[2].size());
			}
		}
		else
		{
			entityMatrix = vector<vector<Entity*>>(2);
			for (int i = 0; i < entities.size(); i++)
			{
				entityMatrix[1].push_back(entities[i]);
				entities[i]->SetLayer(1);
				entities[i]->SetPlaceInLayer((ushort)entityMatrix[1].size());
			}
		}
	}
	entityMatrix.shrink_to_fit();
}

void FightManager::FollowCamera(Entity* ent)
{
	camFollow.push_back(ent);
	ent->AddTag(Tags::CameraFollow);
}

bool FightManager::RemoveEntity(Entity* ent)
{
	return false;
}

void FightManager::RemoveFromFollowCamera(Entity* ent)
{
	if (!ent->HasTag(Tags::CameraFollow))
		return;

	for (int i = 0; i < camFollow.size(); i++)
	{
		if (camFollow[i] == ent)
		{
			for (int j = i + 1; j < camFollow.size(); j++)
			{
				camFollow[j - 1] = camFollow[j];
			}
			camFollow.pop_back();
			break;
		}
	}
}

void FightManager::AddEntity(Entity* ent)
{
	toAdd.push(ent);
}

void FightManager::AddEntity(Entity* ent, ushort layer)
{
	ent->SetLayer(layer);
	toAdd.push(ent);
}

void FightManager::AddToGame(Entity* ent)
{
	entities.push_back(ent);

	entityMatrix[ent->GetLayer()].push_back(ent);
	ent->SetPlaceInLayer((ushort)entityMatrix[ent->GetLayer()].size());
}

std::deque<Entity*>::iterator FightManager::RemoveEntity(std::deque<Entity*>::iterator ent)
{
	std::deque<Entity*>::iterator aux;

	if ((*ent)->HasTag(Tags::IsCharacter))
	{
		auto it = characters.begin();

		while (it != characters.end() && *it != *ent)
		{
			it++;
		}
		if (*it == *ent)
			if (DeleteCharacter(it))
				aux = DeleteEntity(ent);
			else
				aux = ent;
	}
	else
	{
		aux = DeleteEntity(ent);
	}

	return aux;
}

std::deque<Entity*>::iterator FightManager::DeleteEntity(std::deque<Entity*>::iterator ent)
{
	if ((*ent)->HasTag(Tags::CameraFollow))
	{
		auto it = camFollow.begin();

		while (it != camFollow.end())
		{
			if (*it == *ent)
			{
				break;
			}
			it++;
		}
		if (*it == *ent)
			camFollow.erase(it);
	}

	RemoveEntityFromMatrix(*ent);

	Entity* aux = *ent;

	auto auxIt = entities.erase(ent);

	delete aux;

	return auxIt;
}

bool FightManager::DeleteCharacter(std::deque<Character*>::iterator character)
{
	if (!endGame) {

		if (!teammode) {
			addCharacterStats(*character);
			deadTextures.push_back((*character)->getPortrait());
		}
		else
		{
			bool belongsTeam1 = false;

			auto team1It = team1.begin();
			
			while (team1It != team1.end())
			{
				if (**team1It == *character)
				{
					belongsTeam1 = true;
					break;
				}
				team1It++;
			}
			if (belongsTeam1) { // Si pertenece al team1
				vector<ushort>stats;
				stats.push_back((*character)->getDeaths());
				stats.push_back((*character)->getDamageTaken());
				stats.push_back((*character)->getKills());
				team1DeadStats.push_back(stats);
				team1DeadTextures.push_back((*character)->getPortrait());
				team1.erase(team1It);
			}
			else // Si pertenece al team2
			{
				auto team2It = team2.begin();

				while (team2It != team2.end())
				{
					if (**team2It == *character)
					{
						break;
					}
					team2It++;
				}
				vector<ushort>stats;
				stats.push_back((*character)->getDeaths());
				stats.push_back((*character)->getDamageTaken());
				stats.push_back((*character)->getKills());
				team2DeadStats.push_back(stats);
				team2DeadTextures.push_back((*character)->getPortrait());
				team2.erase(team2It);
			}
		}

		listener->RemoveCharacter(*character);
		characters.erase(character);

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
		return true;
	}
	return false;
}

void FightManager::MoveToFront(Entity* ent)
{
	auto it = entities.begin();

	while (it != entities.end())
	{
		if (*it == ent)
		{
			break;
		}
		it++;
	}
	if (it != entities.end())
	{
		entities.erase(it);
		entities.push_front(ent);
	}
}

void FightManager::RemoveEntityFromMatrix(Entity* ent)
{
	for (int i = 0; i < entityMatrix[ent->GetLayer()].size(); i++)
	{
		if (entityMatrix[ent->GetLayer()][i] == ent)
		{
			for (int j = i + 1; j < entityMatrix[ent->GetLayer()].size(); j++)
			{
				entityMatrix[ent->GetLayer()][j - 1] = entityMatrix[ent->GetLayer()][j];
				entityMatrix[ent->GetLayer()][j - 1]->SetPlaceInLayer(j - 1);
			}
			entityMatrix[ent->GetLayer()].pop_back();
			break;
		}
	}
}

void FightManager::KillingBlow()
{
	sdl->soundEffects().at("hitKill").play();
}

void FightManager::ChangeEntityLayer(Entity* ent, ushort newLayer)
{
	RemoveEntityFromMatrix(ent);

	entityMatrix[newLayer].push_back(ent);
	ent->SetLayer(newLayer);
	ent->SetPlaceInLayer((ushort)entityMatrix[newLayer].size());
}

bool FightManager::GetNextEntity(Entity*& ent, ushort layerToIgnore)
{
	if (ent == nullptr)
	{
		short layer = -1;
		for (auto i = 0u; i < entityMatrix.size(); i++)
		{
			if (i != layerToIgnore && entityMatrix[i].size() > 0)
			{
				layer = i;
				break;
			}
		}
		if (layer == -1)
		{
			return false;
		}
		else
		{
			ptrPlace = { layer, -1 };
		}
	}

	do 
	{
		ptrPlace.second++;

		if (ptrPlace.second >= entityMatrix[ptrPlace.first].size())
		{
			ptrPlace.second = 0;

			do
			{
				ptrPlace.first++;

				if (ptrPlace.first >= entityMatrix.size())
				{
					ent = nullptr;
					return false;
				}
			}
			while (ptrPlace.first == layerToIgnore || entityMatrix[ptrPlace.first].size() == 0);
		}
	} while (!entityMatrix[ptrPlace.first][ptrPlace.second]->HasTag(Tags::Hitable) || !entityMatrix[ptrPlace.first][ptrPlace.second]->IsAlive());

	ent = entityMatrix[ptrPlace.first][ptrPlace.second];
	return true;
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
	for (auto i = 0u; i < entities.size(); i++) {
		listener->RemoveCharacter(entities[i]);
		delete entities[i];
	}
	for (auto a : entityMatrix)
	{
		a.clear();
	}
	entityMatrix.clear();
	entities.clear();
	characters.clear();
	camFollow.clear();
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
	if (startticks + 1000u < SDL_GetTicks()) {
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
	return (int)nearbyint(x * stage->getb2ToSDL());
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

void FightManager::GetAllReferencesTo(Entity* toCheck, Entity*& entQue, Character*& chr, Entity*& cam, Entity*& mat)
{
	for (auto i = 0u; i < entities.size(); i++)
		if (entities[i] == toCheck)
			entQue = entities[i];

	for (auto i = 0u; i < characters.size(); i++)
		if (characters[i] == toCheck)
			chr = characters[i];

	for (auto i = 0u; i < camFollow.size(); i++)
		if (camFollow[i] == toCheck)
			cam = camFollow[i];

	for (auto j = 0u; j < entityMatrix.size(); j++)
	{
		for (auto i = 0u; i < entityMatrix[j].size(); i++)
			if (entityMatrix[j][i] == toCheck)
				mat = entityMatrix[j][i];
	}
}

Entity*& FightManager::GetEntityReferenceTo(Entity* toCheck)
{
	for (auto i = 0u; i < entities.size(); i++)
		if (entities[i] == toCheck)
			return entities[i];
}
Character*& FightManager::GetCharacterReferenceTo(Character* toCheck)
{
	for (auto i = 0u; i < characters.size(); i++)
		if (characters[i] == toCheck)
			return characters[i];
}
Entity*& FightManager::GetCameraReferenceTo(Entity* toCheck)
{
	for (auto i = 0u; i < camFollow.size(); i++)
		if (camFollow[i] == toCheck)
			return camFollow[i];
}
Entity*& FightManager::GetMatrixReferenceTo(Entity* toCheck)
{
	for (auto j = 0u; j < entityMatrix.size(); j++)
		for (auto i = 0u; i < entityMatrix[j].size(); i++)
			if (entityMatrix[j][i] == toCheck)
				return entityMatrix[j][i];
}

void FightManager::TakeScreenShot()
{
	SDL_Surface* sshot = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_LockSurface(sshot);
	SDL_Rect a = { 0, 0, width, height };
	SDL_RenderReadPixels(sdl->renderer(), &a, sshot->format->format, sshot->pixels, sshot->pitch);
	string full = "ScreenShot_";
	vector<char> aaaa = vector<char>();
	const int size = 30;
	char sshotName[size] = { "" };
	_strdate_s(sshotName, size);

	for (int i = 0; i < 8; i++)
	{
		if (sshotName[i] == '/')
		{
			for (int j = i; j < 8; j++)
			{
				if (sshotName[j + 1] != '/')
					sshotName[j] = sshotName[j + 1];
				else
					for (int k = j + 1; k < 7; k++)
					{
						sshotName[k] = sshotName[k + 1];
					}
			}
			break;
		}
	}

	full += sshotName;
	full += "_";
	_strtime_s(sshotName, size);

	for (int i = 0; i < 8; i++)
	{
		if (sshotName[i] == ':')
		{
			for (int j = i; j < 8; j++)
			{
				if (sshotName[j + 1] != ':')
					sshotName[j] = sshotName[j + 1];
				else
					for (int k = j + 1; k < 8; k++)
					{
						sshotName[k] = sshotName[k + 1];
					}
			}
			break;
		}
	}

	full += sshotName;
	full += ".jpg";

	char gay[30] = { "" };

	for (int i = 0; i < 30; i++)
	{
		gay[i] = full[i];
	}

	SDL_SaveBMP(sshot, gay);
	SDL_UnlockSurface(sshot);
	SDL_FreeSurface(sshot);
}

void FightManager::LogEverything()
{
}