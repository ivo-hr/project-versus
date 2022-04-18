#include "MenuState.h"
#include "PlayingState.h"
#include "ConfigState.h"
#include "../PlayingState/FightManager.h"


MenuState::MenuState(FightManager* game) : State(game) {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background = &sdl->images().at("menu");

    playBut = new Button(&sdl->images().at("play"), w/2-200, h/ 2+100, 300, 200);
    playBut->setPressTexture(&sdl->images().at("playP"));
}

void MenuState::update() {
    if(ih.isKeyDown(SDLK_SPACE))fmngr->getState()->next();
    if(playBut->mouseClick())fmngr->getState()->next();
}

void MenuState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    playBut->render();
    sdl->presentRenderer();  
}

void MenuState::next() {
    delete playBut;
    cout << "Next State " << endl;
    fmngr->setState(new ConfigState(fmngr));
    delete this;
}
