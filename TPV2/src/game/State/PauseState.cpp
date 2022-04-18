#include "PauseState.h"
#include "MenuState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"

PauseState::PauseState(FightManager* game) : State(game) {
    startTime = sdl->currRealTime();
    Texture* p = &sdl->images().at("pause");
    p->render(750, 400);
    sdl->presentRenderer();
}



void PauseState::update() {
    if (ih.isKeyDown(SDLK_p) && ih.keyDownEvent()) {
        std::cout << "unpause" << std::endl;
        State* tmp = fmngr->getState();
        State* saved = fmngr->getSavedState();
        fmngr->setState(saved);
        fmngr->clearSavedState();
        delete tmp;
    }
}

void PauseState::draw() {
 
}

void PauseState::next() {
    cout << "Next State " << endl;
    delete this;
}