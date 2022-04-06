#include "PlayingState.h"
#include "GameOverState.h"
#include "../PlayingState/FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../../../CharInclude.h"

PlayingState::PlayingState(FightManager* game) : State(game) {
	std::vector<Entity*> entities;
	entities.push_back(new Togo(fmngr, new Vector2D(20, 0), 0));
	entities.push_back(new GatoEspia(fmngr, new Vector2D(30, 0), 1));

	fmngr->StartFight(entities);

}


void PlayingState::update() {
	fmngr->Update();
}

void PlayingState::draw() {

}

void PlayingState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new GameOverState(fmngr));
    delete this;
}
