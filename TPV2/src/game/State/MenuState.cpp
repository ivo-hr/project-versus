#include "MenuState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"


MenuState::MenuState(FightManager* game) : State(game) {
    tex = &sdl->images().at("sdl_logo");

}

void MenuState::update() {
    if(ih.isKeyDown(SDLK_SPACE))fmngr->getState()->next();
}

void MenuState::draw() {
  
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    tex->render(sdl->width(), sdl->height());
    sdl->presentRenderer();
   
}

void MenuState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new PlayingState(fmngr));
    delete this;
}