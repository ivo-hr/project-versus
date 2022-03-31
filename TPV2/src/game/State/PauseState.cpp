#include "PauseState.h"
#include "MenuState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"

PauseState::PauseState(FightManager* game) : State(game) {
   
}



void PauseState::update() {
}

void PauseState::draw() {
 
}

void PauseState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new PlayingState(fmngr));
    delete this;
}