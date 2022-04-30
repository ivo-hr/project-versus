#include "GameOverState.h"
#include "MenuState.h"
#include "../PlayingState/FightManager.h"



GameOverState::GameOverState(FightManager* game, vector<Texture*>winnersTextures, vector<vector<int>>gameStats, int playersInput, vector<int>playersInputV) : State(game) {

    background = &sdl->images().at("gameoverscreen1");
    //fmngr = game;
    winnersTextures_ = winnersTextures;
    playAgain = new Button(&sdl->images().at("play"), ts(150), ts(60), ts(180), ts(130));

    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    int winnerInput=1;
    for (auto i = 0u; i < playersInputV.size(); i++) {
        if (playersInputV[i] == playersInput)winnerInput = i+1;
    }
    string inputString = "P" + to_string(winnerInput) + "P";
    pointer = new PlayerPointer(&sdl->images().at(inputString), ts(200), ts(150), ts(15), ts(15), w, h);
    pointer->setActive(true);
    playersInput_ = playersInput;
    gameStats_ = gameStats;
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
        return;
    }
    if (ih.isKeyDown(SDLK_ESCAPE))fmngr->userExit();
    //if (ih.isKeyDown(SDLK_e))fmngr->getState()->next();
}

void GameOverState::draw() {
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    //showText("GG EASY. Press E to return to Menu", ts(150), ts(100), ts(150), build_sdlcolor(0x112233ff));
    drawGameStats();
    //winnersTextures_[0]->render(ts(100), ts(100));
    playAgain->render();
    pointer->render();
    sdl->presentRenderer();
}

void GameOverState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new MenuState(fmngr));
    delete this;
}

void GameOverState::drawGameStats()
{
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    int numOfplayer = gameStats_.size();
    int dist = (w - ts(50)) / numOfplayer;
    int offset = dist - ts(110);
    for (auto i = 0u; i < numOfplayer; i++) {

        winnersTextures_[numOfplayer - i - 1]->render({ (int)(i * dist + offset), (int)ts(200), (int)ts(50), (int)ts(50) });
        showText(to_string(i + 1), ts(16), (int)(i * dist + offset + ts(-10)), (int)ts(190), build_sdlcolor(0xFFFF0000));

        showText("Deaths: ", ts(8), (int)(i * dist + offset + ts(0)), (int)ts(265), build_sdlcolor(0xFFFF0000));
        showText(to_string(gameStats_[numOfplayer - i - 1][0]), ts(8), (int)(i * dist + offset + ts(80)), (int)ts(265), build_sdlcolor(0xFFFF0000));

        showText("Damage taken: ", ts(8), (int)(i * dist + offset + ts(0)), (int)ts(275), build_sdlcolor(0xFFFF0000));
        showText(to_string(gameStats_[numOfplayer - i - 1][1]), ts(8), (int)(i * dist + offset + ts(120)), (int)ts(275), build_sdlcolor(0xFFFF0000));
    }
}