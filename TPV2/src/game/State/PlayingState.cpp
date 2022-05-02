#include "PlayingState.h"
#include "GameOverState.h"
#include "../PlayingState/FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../../../CharInclude.h"



PlayingState::PlayingState(FightManager* game, vector<int>playersInput, vector<int>characters , int map) : State(game) {
	std::vector<Character*> entities;
	std::vector<Character*> team1;
	std::vector<Character*> team2;
	// input del character (al menos de momento):
	// 0 y 1: teclado
	// 2 y 3: mando NES
	// 4 y 5: mando PS4 o Xbox One
	string s = "resources/config/stage" + to_string(map) + ".json";
	fmngr->LoadStage(s);


	for (auto i = 0u; i < playersInput.size(); i++) {
		switch (characters[i])
		{
		case 0: //zero
			entities.push_back(new NasNas(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i],i));
			break;
		case 1://Gato espia
			entities.push_back(new GatoEspia(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i],i));
			break;
		case 2://Togo
			entities.push_back(new Togo(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i],i));
			break;
		case 3: //Maketo
			entities.push_back(new Makt(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i],i));
			break;
		case 4://Nasnas
			//entities.push_back(new NasNas(fmngr, new Vector2D(20 + i * 10, 0), playersInput[i]));
			break;
		case -1://Aleatorio
			int charac = sdl->rand().nextInt(0, 4);
			switch (charac)
			{
			case 0: //zero
				entities.push_back(new NasNas(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				break;
			case 1://Gato espia
				entities.push_back(new GatoEspia(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				break;
			case 2://Togo
				entities.push_back(new Togo(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				break;
			case 3: //Maketo
				entities.push_back(new Makt(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				break;
			}
		
			break;
		}
	}
	playersInput_ = playersInput;

	fmngr->StartFight(entities);

}

PlayingState::PlayingState(FightManager* game, vector<int> playersInput, vector<int> characters, vector<int> teams, int map) : State(game)
{
	std::vector<Character*> team1;
	std::vector<Character*> team2;
	string s = "resources/config/stage" + to_string(map) + ".json";
	fmngr->LoadStage(s);
	//fmngr->LoadStage("resources/config/stage2.json");

	for (auto i = 0u; i < playersInput.size(); i++) {
		switch (characters[i])
		{
		case 0: //zero
			if (teams[i] == 0)
				team1.push_back(new CharacterZero(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
			else
			{
				team2.push_back(new CharacterZero(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
			}
			break;
		case 1://Gato espia
			if (teams[i] == 0)
				team1.push_back(new GatoEspia(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
			else
			{
				team2.push_back(new GatoEspia(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
			}
			break;
		case 2://Togo
			if (teams[i] == 0)
				team1.push_back(new Togo(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
			else
			{
				team2.push_back(new Togo(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
			}
			break;
		case 3: //Maketo
			if (teams[i] == 0)
				team1.push_back(new Makt(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
			else
			{
				team2.push_back(new Makt(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
			}
			break;
		case 4://Nasnas		
			break;
		case -1://Aleatorio
			int charac = sdl->rand().nextInt(0, 4);
			switch (charac)
			{
			case 0: //zero
				if (teams[i] == 0)
					team1.push_back(new CharacterZero(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				else
				{
					team2.push_back(new CharacterZero(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				}
				break;
			case 1://Gato espia
				if (teams[i] == 0)
					team1.push_back(new GatoEspia(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				else
				{
					team2.push_back(new GatoEspia(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				}
				break;
			case 2://Togo
				if (teams[i] == 0)
					team1.push_back(new Togo(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				else
				{
					team2.push_back(new Togo(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				}
				break;
			case 3: //Maketo
				if (teams[i] == 0)
					team1.push_back(new Makt(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
				else
				{
					team2.push_back(new Makt(fmngr, b2Vec2(20 + i * 10, 0), playersInput[i], i));
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
	cout << "Next State " << endl;
	vector<Texture*>winnersTextures = fmngr->getWinnersTextures();
	vector<vector<int>>gameStats = fmngr->getGameStats();
	fmngr->setState(new GameOverState(fmngr, winnersTextures, gameStats, fmngr->getWinnerInput(), playersInput_));
	delete this;
}
