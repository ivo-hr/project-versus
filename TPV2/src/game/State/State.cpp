#include "State.h"
#include "../PlayingState/FightManager.h"

bool State::doQuit() {
    return false;
}

void State::jump(State* state) {
    fmngr->setState(state);
    delete this;
}

