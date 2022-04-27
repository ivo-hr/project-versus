#include "GameOverState.h"
#include "MenuState.h"
#include "../PlayingState/FightManager.h"


GameOverState::GameOverState(FightManager* game) : State(game) {

    background = &sdl->images().at("selectbg");
}

void GameOverState::update() {
    if (ih.isKeyDown(SDLK_e))fmngr->getState()->next();
}

void GameOverState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    showText("GG EASY. Press E to return to Menu", ts(150), ts(100), ts(150), build_sdlcolor(0x112233ff));
    sdl->presentRenderer();
}
void GameOverState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new MenuState(fmngr));
    delete this;
}