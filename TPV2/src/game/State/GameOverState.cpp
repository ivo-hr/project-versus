#include "GameOverState.h"
#include "MenuState.h"
#include "../PlayingState/FightManager.h"


GameOverState::GameOverState(FightManager* game, vector<Texture*>winnersTextures, int playersInput) : State(game) {

    background = &sdl->images().at("selectbg");
    //fmngr = game;
    winnersTextures_ = winnersTextures;
    playAgain = new Button(&sdl->images().at("play"), ts(150), ts(40), ts(200), ts(150));

    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    pointer = new PlayerPointer(&sdl->images().at("P1P"), ts(200), ts(150), ts(15), ts(15), w, h);
    pointer->setActive(true);
    playersInput_ = playersInput;
}

GameOverState::~GameOverState()
{
    delete playAgain;
    delete pointer;
}

void GameOverState::update() {

    switch (playersInput_)
    {
    case -1:
        if (ih.isKeyDown(SDLK_w))pointer->move(0);
        if (ih.isKeyDown(SDLK_s))pointer->move(1);
        if (ih.isKeyDown(SDLK_a))pointer->move(2);
        if (ih.isKeyDown(SDLK_d))pointer->move(3);
        break;
    case -2:
        if (ih.isKeyDown(SDLK_UP))pointer->move(0);
        if (ih.isKeyDown(SDLK_DOWN))pointer->move(1);
        if (ih.isKeyDown(SDLK_LEFT))pointer->move(2);
        if (ih.isKeyDown(SDLK_RIGHT))pointer->move(3);
        break;
    default:
        if (ih.xboxGetAxesState(playersInput_, 1) == -1 || ih.xboxGetDpadState(playersInput_, 0))pointer->move(0);
        if (ih.xboxGetAxesState(playersInput_, 1) == 1 || ih.xboxGetDpadState(playersInput_, 2))pointer->move(1);
        if (ih.xboxGetAxesState(playersInput_, 0) == -1 || ih.xboxGetDpadState(playersInput_, 3))pointer->move(2);
        if (ih.xboxGetAxesState(playersInput_, 0) == 1 || ih.xboxGetDpadState(playersInput_, 1))pointer->move(3);
        break;
    }

    bool enter = false;
    switch (playersInput_)
    {
    case -1:
        if (ih.isKeyDown(SDLK_e))enter = true;
        break;
    case -2:
        if (ih.isKeyDown(SDLK_l))enter = true;
        break;
    default:
        if (ih.xboxGetButtonState(playersInput_, SDL_CONTROLLER_BUTTON_B))enter = true;
        break;
    }
    if (playAgain->pointerClick(pointer->getRect()) && enter) {
        fmngr->getState()->next();
    }

    //if (ih.isKeyDown(SDLK_e))fmngr->getState()->next();
}

void GameOverState::draw() {
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    showText("GG EASY. Press E to return to Menu", ts(150), ts(100), ts(150), build_sdlcolor(0x112233ff));
    winnersTextures_[0]->render(ts(100), ts(100));
    playAgain->render();
    pointer->render();
    sdl->presentRenderer();
}
void GameOverState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new MenuState(fmngr));
    delete this;
}