#include "ConfigState.h"
#include "PlayingState.h"
#include "ExitState.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"
#include "../Utils/PlayerConfigs.h"
#include "../Utils/PlayerPointer.h"
#include "../Utils/PlayerSelectRect.h"
#include <iostream>
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Utils/Button.h"
#include "../Utils/PlayerConfigs.h"

#include "ConfigurationState.h"

using namespace std;

vector<char> ConfigState::AuxFunc(char inp)
{
	vector<char> inputsAux = vector<char>();
	inputsAux.push_back(inp);
	return inputsAux;
}

ConfigState::ConfigState(FightManager* game, const vector<char>& inputs) : State(game), numOfplayer((ushort)std::max((ushort)inputs.size(), (ushort)2))
{
	int w = fmngr->GetActualWidth();
	int h = fmngr->GetActualHeight();

	stageTextures[0] = &sdl->images().at("fondo");
	stageTextures[1] = &sdl->images().at("mazmorra");
	stageTextures[2] = &sdl->images().at("night");

	int dist = (w * 12 / 13) / numOfplayer;
	int offset = dist - w / 13;

	ushort i = 0;
	for (; i < inputs.size(); i++)
	{
		string a = "P" + std::to_string((i + 1)) + "P";
		playerPointers.push_back(new PlayerPointer(&sdl->images().at(a), i * dist + offset, h / 2, w, h, inputs[i]));
		playerPointers[i]->setActive(true);
	}
	for (; i < 4; i++)
	{
		string a = "P" + std::to_string((i + 1)) + "P";
		playerPointers.push_back(new PlayerPointer(&sdl->images().at(a), i * dist + offset, h / 2, w, h));
	}

	InitAllButtons(w, h);

	playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P1")));
	playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P2")));
	playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P3")));
	playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P4")));

	nMandos = SDL_NumJoysticks();
	usedPad.resize(4);
	selected.resize(4);
	usedKeyboard.resize(2);

	charactersSelect.resize(inputs.size());
	playerInput.resize(inputs.size());

	for (ushort i = 0; i < inputs.size(); i++)
	{
		playerInput[i] = (char)inputs[i];
		playerTexture[i]->setgotInput(true);
		if (inputs[i] >= 0) { usedPad[inputs[i]] = true; playerTexture[i]->setFront(&sdl->images().at("Mando")); }
		else if (inputs[i] == -1) {
			usedKeyboard[0] = true; playerTexture[i]->setFront(&sdl->images().at("k1"));
		}
		else if (inputs[i] == -2) {
			usedKeyboard[1] = true; playerTexture[i]->setFront(&sdl->images().at("k2"));
		}
	}

	sdl->musics().at("sawtines").play();
	SDL_ShowCursor(1);
}

ConfigState::ConfigState(FightManager* game, char fInput) : ConfigState(game, AuxFunc(fInput)) { }

ConfigState::~ConfigState()
{
	delete play;
	delete teammode;
	delete normalmode;
	for (auto e : playerPointers) delete e;
	for (auto e : playerTexture) delete e;
	for (auto e : charactTexture) delete e;
	for (auto e : buttons) if (e) delete e;
	for (auto& e : p)
	{
		for (auto a : e)delete a;
	}
	for (auto e : maps)delete e;
}

void ConfigState::InitAllButtons(int w, int h)
{
	initcharact();

	buttons[7] = new Button(&sdl->images().at("pB"), (int)(w * 14 / 15), (int)(h - w * 2.4f / 15), (int)(w / 15), playerPointers);
	buttons[7]->SetOnClick([this]() { AddPlayer(); });

	buttons[8] = new Button(&sdl->images().at("mB"), (int)(w * 14 / 15), (int)(h - w * 1.2f / 15), (int)(w / 15), playerPointers);
	buttons[8]->SetOnClick([this]() { RemovePlayer(); });

	buttons[9] = new Button(&sdl->images().at("ConfigBut"), (w * 20 / 21) + 1, -1, w / 21, playerPointers);
	buttons[9]->SetOnClick([this]() { OpenConfig(); });

	play = new PlayButton(&sdl->images().at("play"), 0, 0, w, h, playerPointers);
	play->SetOnClick([this]() { fmngr->getState()->next(); });


	normalmode = new ToggleButton(&sdl->images().at("MNormal"), (w / 2) + (h / 168), h / 168, h / 7, h / 14, playerPointers);
	normalmode->SetOnClick([this]()
		{
			TeamModebool = !TeamModebool;
			normalmode->SetEnabled(!TeamModebool);
			teammode->SetEnabled(TeamModebool);

			sdl->soundEffects().at("uiMov").play();
		});
	normalmode->SetEnabled(true);

	teammode = new ToggleButton(&sdl->images().at("MTeam"), (w / 2) + h / 84 + h / 7, h / 168, h / 7, h / 14, playerPointers);
	teammode->SetOnClick([this]()
		{
			TeamModebool = !TeamModebool;
			normalmode->SetEnabled(!TeamModebool);
			teammode->SetEnabled(TeamModebool);

			sdl->soundEffects().at("uiMov").play();
		});
	teammode->SetEnabled(false);

	configTeamChoose();
	initMapBut();
}

void ConfigState::update()
{
	if (ih->isKeyDown(SDLK_ESCAPE) && ih->keyDownEvent()) {
		if (fmngr->getSavedState() == nullptr) {
			//pause
			fmngr->saveState(fmngr->getState());
			fmngr->setState(new ExitState(fmngr));
			return;
		}
	}

	if (!selectMap)
	{
		searchInput();
		movePointers();
		checkButtonPointerClick();
		checkPlayerReady();
		for (auto e : buttons)
		{
			if (e)
				e->update();
		}
		if (fmngr->getState() != this)
			return;
		teammode->update();
		normalmode->update();
		for (auto& e : p)
		{
			for (auto& t : e)
			{
				t->update();
			}
		}
		play->update();
	}
	else
	{
		movePointers();
		for (auto e : maps)
		{
			e->update();
		}
		buttons[9]->update();
		if (mapChosen >= 0) selectMap = false;
	}
}

void ConfigState::draw()
{

	if (selectMap)
	{
		mapMenuRender();
	}
	else
	{
		playerMenuRender();
	}

	sdl->presentRenderer();
}

void ConfigState::next()
{

	if (!TeamModebool)
	{
		fmngr->setState(new PlayingState(fmngr, playerInput, charactersSelect, mapChosen + 1)); //3 para el de noche
	}
	else
	{
		fmngr->setState(new PlayingState(fmngr, playerInput, charactersSelect, charactersTeam, mapChosen + 1));
	}

	delete this;
}

void ConfigState::setPointer(short input)
{
	int w = fmngr->GetActualWidth();
	int dist = (w * 12 / 13) / numOfplayer;
	int offset = dist - w / 13;
	playerPointers[playerInput.size() - 1]->setActive(true);
	playerPointers[playerInput.size() - 1]->setPosition(((int)playerInput.size() - 1) * dist + offset, (int)(fmngr->GetActualHeight() / 2.f));
	playerPointers[playerInput.size() - 1]->setInput(input);
}
void ConfigState::configTeamChoose()
{
	int w = fmngr->GetActualWidth();
	int h = fmngr->GetActualHeight();

	charactersTeam.resize(4);

	// ChangeTeamButtons
	{
		vector<ToggleButton*> p1;
		p1.push_back(new ToggleButton(&sdl->images().at("T1"), 0, 0, w / 30, w / 30, playerPointers));
		p1[0]->SetOnClick([this]() { ChangeTeam(0, true); });

		p1.push_back(new ToggleButton(&sdl->images().at("T2"), 0, 0, w / 30, w / 30, playerPointers));
		p1[1]->SetOnClick([this]() { ChangeTeam(0, false); });
		p.push_back(p1);
		p1.clear();

		p1.push_back(new ToggleButton(&sdl->images().at("T1"), 0, 0, w / 30, w / 30, playerPointers));
		p1[0]->SetOnClick([this]() { ChangeTeam(1, true); });

		p1.push_back(new ToggleButton(&sdl->images().at("T2"), 0, 0, w / 30, w / 30, playerPointers));
		p1[1]->SetOnClick([this]() { ChangeTeam(1, false); });

		p.push_back(p1);
		p1.clear();

		p1.push_back(new ToggleButton(&sdl->images().at("T1"), 0, 0, w / 30, w / 30, playerPointers));
		p1[0]->SetOnClick([this]() { ChangeTeam(2, true); });

		p1.push_back(new ToggleButton(&sdl->images().at("T2"), 0, 0, w / 30, w / 30, playerPointers));
		p1[1]->SetOnClick([this]() { ChangeTeam(2, false); });

		p.push_back(p1);
		p1.clear();

		p1.push_back(new ToggleButton(&sdl->images().at("T1"), 0, 0, w / 30, w / 30, playerPointers));
		p1[0]->SetOnClick([this]() { ChangeTeam(3, true); });

		p1.push_back(new ToggleButton(&sdl->images().at("T2"), 0, 0, w / 30, w / 30, playerPointers));
		p1[1]->SetOnClick([this]() { ChangeTeam(3, false); });

		p.push_back(p1);
		p1.clear();
	}

	for (int i = 0; i < 4; i++)
	{
		ChangeTeam(i, i < 2);
	}
}

void ConfigState::searchInput()
{
	//Si hay algun input pendiente , buscarlo
	if (playerInput.size() < numOfplayer) {
		if (SDL_NumJoysticks() == nMandos)
		{
			for (auto i = 0; i < SDL_NumJoysticks(); i++) {
				if (ih->xboxGetAxesState(i, 1) == -1 && !usedPad[i]) {
					usedPad[i] = true;
					playerInput.push_back(i);
					setPointer(i);
					playerTexture[playerInput.size() - 1]->setgotInput(true);
					playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("Mando"));
					charactersSelect.resize(playerInput.size());
					return;
				}
			}
		}
		else if (SDL_NumJoysticks() > nMandos) { //se ha enchufado nuevo mando
			vector<char> aux = playerInput;
			for (auto i = 0; i < SDL_NumJoysticks(); i++) {
				if (ih->xboxGetAxesState(i, 1) == -1) {
					bool nes = false;
					for (ushort j = 0u; j < aux.size(); j++)
					{
						if (i == aux[j]) {
							nes = true;
							j = (ushort)aux.size();
						}
					}
					if (nes) {
						for (ushort x = 0u; x < aux.size(); x++)
						{
							if (aux[x] >= 0)
							{
								aux[x] = aux[x] + 1;
								usedPad[aux[x]] = true;
							}
						}
					}
					usedPad[i] = true;
					aux.push_back(i);
					playerInput = aux;
					setPointer(i);
					playerTexture[playerInput.size() - 1]->setgotInput(true);
					playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("Mando"));
					charactersSelect.resize(playerInput.size());
					nMandos++;
					return;
				}
			}
		}
		if (ih->isKeyDown(playerPrefs->Keyboard2Up()) && !usedKeyboard[1]) {
			usedKeyboard[1] = true;
			playerInput.push_back(-2);
			setPointer(-2);
			playerTexture[playerInput.size() - 1]->setgotInput(true);
			playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("k2"));
		}
		if (ih->isKeyDown(playerPrefs->Keyboard1Up()) && !usedKeyboard[0]) {
			usedKeyboard[0] = true;
			playerInput.push_back(-1);
			setPointer(-1);
			playerTexture[playerInput.size() - 1]->setgotInput(true);
			playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("k1"));
		}
		charactersSelect.resize(playerInput.size());
	}
}

void ConfigState::movePointers()
{
	//Movimiento de los punteros
	for (auto i = 0; i < playerInput.size(); i++)
	{
		playerPointers[i]->update();
	}
}

void ConfigState::checkButtonPointerClick()
{
	//Seleccion del personaje
	for (auto i = 0; i < playerInput.size(); i++) {
		switch (playerInput[i])
		{
		case -1:
			if (ih->isKeyDown(playerPrefs->Keyboard1Special())) selected[i] = false;
			break;
		case -2:
			if (ih->isKeyDown(playerPrefs->Keyboard2Special())) selected[i] = false;
			break;
		default:
			if (ih->xboxGetButtonState(playerInput[i], playerPrefs->ControllerSpecial())) selected[i] = false;
			break;
		}
	}
}

void ConfigState::OpenConfig()
{
	if (fmngr->getSavedState() == nullptr)
	{
		keyRelease = false;
		//pause
		fmngr->saveState(fmngr->getState());
		fmngr->setState(new ConfigurationState(fmngr, playerInput[0]));
	}
	else
	{
		keyRelease = false;
		State* tmp = fmngr->getState();
		State* saved = fmngr->getSavedState();
		fmngr->setState(saved);
		fmngr->saveState(tmp);
	}
}

void ConfigState::checkPlayerReady()
{
	//Empezar la partida
	for (auto i = 0u; i < numOfplayer; i++) {
		if (!selected[i]) {
			ready = false;
			play->reset();
			return;
		}
	}
	if (ready)return;
	ready = true;
	sdl->soundEffects().at("uiPlay").play(0, 1);
}

void ConfigState::playerMenuRender()
{
	background = &sdl->images().at("selectbg");
	charselbg = &sdl->images().at("selectcharbg");
	int w = fmngr->GetActualWidth();
	int h = fmngr->GetActualHeight();
	sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
	background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });

	if (mapChosen >= 0)
		stageTextures[mapChosen]->render(build_sdlrect(w * 19.f / 22.f, 5, w / 18.f, w / 24.f));

	for (auto c = 0u; c < 10; c++) {
		int dist = (w * 9 / 10) / 5;
		int offset = dist - w / 10;
		int j = c / 5;

		charselbg->render({ (int)(c % 5 * dist + offset), (int)(((w * 3 / 24) * j) + w / 12), (int)w / 12, (int)w / 12 });
		showText(charName[c], w / 80, (int)(c % 5 * dist + offset) - w / 24, (int)((((w * 3 / 24.f) * j) + w * 1.02f / 6)), 0x33FFE900);
	}
	int dist = (w * 9 / 10) / numOfplayer;
	int offset = dist - (int)w / 5;
	for (auto i = 0u; i < numOfplayer; i++) {
		playerTexture[i]->render((int)(i * dist + offset), (int)h * 2 / 3 + h / 24, (int)w / 5, (int)h / 4);
		if (selected[i])
			showText("Selected", w / 70, (int)(i * dist + offset + w / 25), (int)h * 2 / 24 + h * 2 / 3, 0x55FF5500);
	}

	if (TeamModebool) {
		for (auto i = 0u; i < playerInput.size(); i++) {

			for (int j = 0; j < 2; j++)
			{
				p[i][j]->setX((int)(i * dist + offset + w / 17.5f + j * (w / 20.f)));
				p[i][j]->setY((int)(h * 18.f / 20.f));
				p[i][j]->render();
			}
		}
	}
	for (auto b : buttons)
	{
		if (b)
			b->render();
	}
	normalmode->render();
	teammode->render();
	if (ready) {
		play->render();
	}

	for (auto e : playerPointers)e->render();
}


void ConfigState::initMapBut()
{

	int imgW = fmngr->GetActualWidth() / 6, imgH = fmngr->GetActualWidth() / 8;

	int totalW = (fmngr->GetActualWidth() * 3 / 5) + imgW;
	int offsetX = (fmngr->GetActualWidth() - totalW) / 2;

	int totalH = (fmngr->GetActualHeight() / 2) + imgH;
	int offsetY = (fmngr->GetActualHeight() - totalH) / 2;

	int i = 0;
	maps.push_back(new Button(stageTextures[0], offsetX + (fmngr->GetActualWidth() * (i % 4)) / 5, offsetY + fmngr->GetActualHeight() * (i / 4), imgW, imgH, playerPointers));
	maps[i]->SetOnClick([this]()
		{
			mapChosen = 0;
			sdl->soundEffects().at("uiSelect").play();
			SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
		});
	i++;
	maps.push_back(new Button(&sdl->images().at("mazmorra"), offsetX + (fmngr->GetActualWidth() * (i % 4)) / 5, offsetY + fmngr->GetActualHeight() * (i / 4), imgW, imgH, playerPointers));
	maps[i]->SetOnClick([this]()
		{
			mapChosen = 1;
			sdl->soundEffects().at("uiSelect").play();
			SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
		});
	i++;
	maps.push_back(new Button(&sdl->images().at("night"), offsetX + (fmngr->GetActualWidth() * (i % 4)) / 5, offsetY + fmngr->GetActualHeight() * (i / 4), imgW, imgH, playerPointers));
	maps[i]->SetOnClick([this]()
		{
			mapChosen = 2;
			sdl->soundEffects().at("uiSelect").play();
			SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
		});
}


void ConfigState::mapMenuRender()
{
	background = &sdl->images().at("mapbg");
	int w = fmngr->GetActualWidth();
	int h = fmngr->GetActualHeight();
	sdl->clearRenderer();
	background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
	int dist = (w * 12 / 13) / numOfplayer;
	int offset = dist - w / 7;
	for (auto e : maps)
	{
		e->render();
	}
	buttons[9]->render();

	for (auto e : playerPointers)e->render();
}

void ConfigState::initcharact()
{
	int w = fmngr->GetActualWidth();
	int h = fmngr->GetActualHeight();
	int dist = (int)(((float)w * 9.f / 10.f) / 5.f);
	int offset = (int)(dist - (float)w / 10.f + (float)w / 68.7f);

	int distY = (int)((float)w * 3.f / 24.f);
	int offsetY = (int)((float)w / 12.f + (float)w / 70.f);

	int buttonSize = (int)((float)w / 15.4f);
	//c % 4 * dist + offset), (int)((ts(80) * j) + ts(50));

   // { 
   // (int)(c % 5 * dist + offset), 
   // (int)(((w * 3 / 24) * j) + w / 12), 
   //     (int)w / 12, (int)w / 12 }

	buttons[0] = new Button(&sdl->images().at("nasNasSelect"), offset, offsetY, buttonSize, playerPointers);
	buttons[0]->SetOnPointerClick([this](int as) { SelectCharacter(as, "nasNasSelect", 0); });
	charName.push_back("    NasNas");

	buttons[1] = new Button(&sdl->images().at("blinkMasterSelect"), dist + offset, offsetY, buttonSize, playerPointers);
	buttons[1]->SetOnPointerClick([this](int as) { SelectCharacter(as, "blinkMasterSelect", 1); });
	charName.push_back(" Blink Master");

	buttons[2] = new Button(&sdl->images().at("maktSelect"), dist * 2 + offset, offsetY, buttonSize, playerPointers);
	buttons[2]->SetOnPointerClick([this](int as) { SelectCharacter(as, "maktSelect", 2); });
	charName.push_back("  Makt Fange");

	buttons[3] = new Button(&sdl->images().at("dinoSoulsSelect"), dist * 3 + offset, offsetY, buttonSize, playerPointers);
	buttons[3]->SetOnPointerClick([this](int as) { SelectCharacter(as, "dinoSoulsSelect", 3); });
	charName.push_back("     Togo");

	buttons[4] = new Button(&sdl->images().at("yunoSelect"), dist * 4 + offset, offsetY, buttonSize, playerPointers);
	buttons[4]->SetOnPointerClick([this](int as) { SelectCharacter(as, "yunoSelect", 4); });
	charName.push_back("     Yuno");

	buttons[5] = new Button(&sdl->images().at("melvinSelect"), offset, distY + offsetY, buttonSize, playerPointers);
	buttons[5]->SetOnPointerClick([this](int as) { SelectCharacter(as, "melvinSelect", 5); });
	charName.push_back("    Melvin");

	charName.push_back(" Coming Soon");
	charName.push_back(" Coming Soon");
	charName.push_back(" Coming Soon");

	buttons[6] = new Button(&sdl->images().at("aleatorioSelect"), dist * 4 + offset, distY + offsetY, buttonSize, playerPointers);
	buttons[6]->SetOnPointerClick([this](int as) { SelectCharacter(as, "aleatorioSelect", -1); });
	charName.push_back("    Random");
}

void ConfigState::SelectCharacter(int i, const string& name, char character)
{
	if (!selected[i])
	{
		playerTexture[i]->setFront(&sdl->images().at(name));
		charactersSelect[i] = character;
		keyRelease = false;
		selected[i] = true;

		sdl->soundEffects().at("uiMov").play();
	}
}
void ConfigState::RemovePlayer()
{
	if (numOfplayer > 2)
	{
		numOfplayer--;

		sdl->soundEffects().at("uiMov").play();

		if (playerInput.size() > numOfplayer) {
			switch (playerInput[playerInput.size() - 1])
			{
			case -1:
				usedKeyboard[0] = false;
				break;
			case -2:
				usedKeyboard[1] = false;
				break;
			default:
				usedPad[playerInput[playerInput.size() - 1]] = false;
				break;
			}
			playerInput.resize(numOfplayer);
			playerPointers[playerInput.size()]->setActive(false);
			playerTexture[playerInput.size()]->setgotInput(false);
		}
		charactersSelect.resize(playerInput.size());
		keyRelease = false;
	}
}
void ConfigState::AddPlayer()
{
	if (numOfplayer < 4)
	{
		numOfplayer++;
		charactersSelect.resize(playerInput.size());
		keyRelease = false;

		sdl->soundEffects().at("uiMov").play();
	}
}
void ConfigState::ChangeTeam(int pl, bool t1)
{
	if (t1)
	{
		p[pl][1]->SetEnabled(false);
		p[pl][0]->SetEnabled(true);
		p[pl][1]->SetActive(true);
		p[pl][0]->SetActive(false);
		charactersTeam[pl] = 0;
	}
	else
	{
		p[pl][1]->SetEnabled(true);
		p[pl][0]->SetEnabled(false);
		p[pl][1]->SetActive(false);
		p[pl][0]->SetActive(true);
		charactersTeam[pl] = 1;
	}
}
