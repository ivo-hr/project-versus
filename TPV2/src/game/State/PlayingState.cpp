#include "PlayingState.h"
#include "GameOverState.h"
#include "../PlayingState/FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../../../CharInclude.h"
#include "../../utils/CheckML.h"
#include "../Utils/PlayerConfigs.h"



PlayingState::PlayingState(FightManager* game, vector<char>playersInput, vector<short>characters , ushort map) : State(game) {
	std::vector<Character*> entities;
	std::vector<Character*> team1;
	std::vector<Character*> team2;
	// input del character (al menos de momento):
	// 0 y 1: teclado
	// 2 y 3: mando NES
	// 4 y 5: mando PS4 o Xbox One
	string s = "resources/config/Stages/stage" + to_string(map) + ".json";
	fmngr->LoadStage(s);


	for (auto i = 0u; i < playersInput.size(); i++) {
		switch (characters[i])
		{
		case 0: //zero
			entities.push_back(new NasNas(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i],i));
			break;
		case 1://Gato espia
			entities.push_back(new GatoEspia(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i],i));
			break;
		case 2://Togo
			entities.push_back(new Togo(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i],i));
			break;
		case 3: //Maketo
			entities.push_back(new Makt(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i],i));
			break;
			
		case 4://Yuno
			entities.push_back(new Yuno(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			break;
		case -1://Aleatorio
			int charac = sdl->rand().nextInt(0, 11);

#ifdef _DEBUG
			charac = sdl->rand().nextInt(10, 11);		//Si estamos en debug siempre sale Ciro
#endif // _DEBUG

			switch (charac)
			{
			case 0: //nasnas
			case 1:
				entities.push_back(new NasNas(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				break;
			case 2://Gato espia
			case 3:
				entities.push_back(new GatoEspia(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				break;
			case 4://Togo
			case 5:
				entities.push_back(new Togo(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				break;
			case 6: //Maketo
			case 7:
				entities.push_back(new Makt(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				break;
			case 8: //Yuno
			case 9:
				entities.push_back(new Yuno(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				break;
			case 10: //Zero
				entities.push_back(new CharacterZero(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				break;
			}
			break;
		}
	}
	playersInput_ = playersInput;

	SDL_ShowCursor(0);

	fmngr->StartFight(entities);

}

PlayingState::PlayingState(FightManager* game, vector<char> playersInput, vector<short> characters, vector<short> teams, ushort map) : State(game)
{
	std::vector<Character*> team1;
	std::vector<Character*> team2;
	string s = "resources/config/Stages/stage" + to_string(map) + ".json";
	fmngr->LoadStage(s);
	//fmngr->LoadStage("resources/config/stage2.json");

	for (auto i = 0u; i < playersInput.size(); i++) {
		switch (characters[i])
		{
		case 0: //zero
			if (teams[i] == 0)
				team1.push_back(new CharacterZero(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			else
			{
				team2.push_back(new CharacterZero(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			}
			break;
		case 1://Gato espia
			if (teams[i] == 0)
				team1.push_back(new GatoEspia(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			else
			{
				team2.push_back(new GatoEspia(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			}
			break;
		case 2://Togo
			if (teams[i] == 0)
				team1.push_back(new Togo(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			else
			{
				team2.push_back(new Togo(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			}
			break;
		case 3: //Maketo
			if (teams[i] == 0)
				team1.push_back(new Makt(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			else
			{
				team2.push_back(new Makt(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			}
			break;
		case 4://Yuno
			if (teams[i] == 0)
				team1.push_back(new Yuno(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			else
			{
				team2.push_back(new Yuno(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
			}
			break;
		case -1://Aleatorio
			ushort charac = sdl->rand().nextInt(0, 9);
			switch (charac)
			{
			case 0: //Nasnas
			case 1:
				if (teams[i] == 0)
					team1.push_back(new NasNas(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				else
				{
					team2.push_back(new NasNas(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				}
				break;
			case 2://Gato espia
			case 3:
				if (teams[i] == 0)
					team1.push_back(new GatoEspia(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				else
				{
					team2.push_back(new GatoEspia(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				}
				break;
			case 4://Togo
			case 5:
				if (teams[i] == 0)
					team1.push_back(new Togo(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				else
				{
					team2.push_back(new Togo(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				}
				break;
			case 6: //Maketo
			case 7:
				if (teams[i] == 0)
					team1.push_back(new Makt(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				else
				{
					team2.push_back(new Makt(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				}
				break;
			case 8:
			case 9:
				if (teams[i] == 0)
					team1.push_back(new Yuno(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				else
				{
					team2.push_back(new Yuno(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				}
			case 10: //Zero
				if (teams[i] == 0)
					team1.push_back(new CharacterZero(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				else
				{
					team2.push_back(new CharacterZero(fmngr, b2Vec2((float)(20 + i * 10), 0.f), playersInput[i], i));
				}
				break;
			}
			break;
		}
		playersInput_ = playersInput;
	}
	fmngr->StartFight(team1, team2);
}

PlayingState::~PlayingState()
{
	fmngr->onNewGame();
}


void PlayingState::update() {
	fmngr->Update();
}

void PlayingState::draw() {

}

void PlayingState::next() {
	vector<Texture*>winnersTextures = fmngr->getWinnersTextures();
	vector<vector<ushort>>gameStats = fmngr->getGameStats();
	fmngr->setState(new GameOverState(fmngr, winnersTextures, gameStats, fmngr->getWinnerInput(), playersInput_));
	delete this;
}
