#include "PlayingState.h"
#include "GameOverState.h"
#include "../PlayingState/FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../../../CharInclude.h"

PlayingState::PlayingState(FightManager* game, vector<int>playersInput, vector<int>characters) : State(game) {
	std::vector<Character*> entities;
	std::vector<Character*> team1;
	std::vector<Character*> team2;
	// input del character (al menos de momento):
	// 0 y 1: teclado
	// 2 y 3: mando NES
	// 4 y 5: mando PS4 o Xbox One

	fmngr->LoadStage("resources/config/stage2.json");


	for (auto i = 0u; i < playersInput.size(); i++) {
		switch (characters[i])
		{
		case 0: //zero
			entities.push_back(new CharacterZero(fmngr, new Vector2D(20 + i * 10, 0), playersInput[i]));
			break;
		case 1://Gato espia
			entities.push_back(new GatoEspia(fmngr, new Vector2D(20 + i * 10, 0), playersInput[i]));
			break;
		case 2://Togo
			entities.push_back(new Togo(fmngr, new Vector2D(20 + i * 10, 0), playersInput[i]));
			break;
		case 3: //Maketo
			entities.push_back(new Makt(fmngr, new Vector2D(20 + i * 10, 0), playersInput[i]));
			break;
		case 4://Nasnas
			//entities.push_back(new GatoEspia(fmngr, new Vector2D(20 + i * 10, 0), player[i]));
			break;
		case 5://Aleatorio
			//entities.push_back(new Togo(fmngr, new Vector2D(20 + i * 10, 0), player[i]));
			break;
		}
	}
	playersInput_ = playersInput;

	fmngr->StartFight(entities);

}


void PlayingState::update() {
	fmngr->Update();
}

void PlayingState::draw() {

}

void PlayingState::next() {
	cout << "Next State " << endl;
	vector<Texture*>winnersTextures = fmngr->getWinnersTextures();
	fmngr->setState(new GameOverState(fmngr, winnersTextures, fmngr->getWinnerInput() , playersInput_));
	delete this;
}
