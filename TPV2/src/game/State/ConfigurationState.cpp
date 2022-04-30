#include "ConfigurationState.h"

#include "../PlayingState/FightManager.h"


ConfigurationState::ConfigurationState(FightManager* game) : State(game) {
    startTime = sdl->currRealTime();
    Texture* p = &sdl->images().at("pause");
    p->render(750, 400);
    sdl->presentRenderer();
}



void ConfigurationState::update() {
    
    if (ih.isKeyDown(SDLK_p) && ih.keyDownEvent()) {
        std::cout << "unpause" << std::endl;
        State* tmp = fmngr->getState();
        State* saved = fmngr->getSavedState();
        fmngr->setState(saved);
        fmngr->clearSavedState();
        delete tmp;
    }
}

void ConfigurationState::draw() {

}

void ConfigurationState::next() {
    cout << "Next State " << endl;
    delete this;
}