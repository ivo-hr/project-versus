#include "ConfigState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"


ConfigState::ConfigState(FightManager* game) : State(game), numOfplayer(2) {
    background = &sdl->images().at("fondo");
}

void ConfigState::update() {
    if (ih.isKeyDown(SDLK_SPACE))fmngr->getState()->next();

    if (ih.isKeyDown(SDLK_LEFT) && ih.keyDownEvent() && numOfplayer > 2) numOfplayer--;

    if (ih.isKeyDown(SDLK_RIGHT) && ih.keyDownEvent()&& numOfplayer < 4) numOfplayer++;
}

void ConfigState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    
    for (auto i = 0u; i < numOfplayer; i++) {
        showText(" Player "+ to_string(i+1) , 48 , 400, h/2+i*100, build_sdlcolor(0x112233ff));
    }
    sdl->presentRenderer();
}

void ConfigState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new PlayingState(fmngr));
    delete this;
}
