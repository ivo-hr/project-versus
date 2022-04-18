#include "MenuState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"


MenuState::MenuState(FightManager* game) : State(game) {
    background = &sdl->images().at("menu");
    playBut = new Button(&sdl->images().at("star"), fmngr->GetActualWidth() / 2, fmngr->GetActualHeight() / 2, 100, 100);
}

void MenuState::update() {
    if(ih.isKeyDown(SDLK_SPACE))fmngr->getState()->next();
    if(playBut->mouseClick())fmngr->getState()->next();
}

void MenuState::draw() {
  
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    playBut->render();
    sdl->presentRenderer();
   
}

void MenuState::next() {
    delete playBut;
    cout << "Next State " << endl;
    fmngr->setState(new PlayingState(fmngr));
    delete this;
}
