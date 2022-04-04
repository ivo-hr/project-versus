#include "GameOverState.h"
#include "MenuState.h"
#include "../PlayingState/FightManager.h"


GameOverState::GameOverState(FightManager* game) : State(game) {
 
}

void GameOverState::update() {
}

void GameOverState::draw() {
  
}
void GameOverState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new MenuState(fmngr));
    delete this;
}