#include "GameOverState.h"
#include "MenuState.h"
#include "ExitState.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"


GameOverState::GameOverState(FightManager* game, vector<Texture*>winnersTextures, vector<vector<int>>gameStats, int playersInput, vector<int>playersInputV) : State(game) {

    background = &sdl->images().at("gameoverscreen1");
    //fmngr = game;
    winnersTextures_ = winnersTextures;
    playAgain = new Button(&sdl->images().at("playagain"), ts(180), ts(70), ts(150), ts(100));

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

    sdl->musics().at("win").play();
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
    if (ih.isKeyDown(SDLK_ESCAPE) && ih.keyDownEvent()) {
        if (fmngr->getExitState() == nullptr) {
            //pause
            fmngr->saveExitState(fmngr->getState());
            fmngr->setState(new ExitState(fmngr));
            return;
        }
    }
    //if (ih.isKeyDown(SDLK_e))fmngr->getState()->next();
}

void GameOverState::draw() {
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    drawGameStats();
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
    int dist = (w ) / numOfplayer;
    int offset = (w/2) / numOfplayer -ts(50);
    if (fmngr->getTeammode()) {
        showText("1", ts(16), (int)(dist / 2 + 3 * offset), (int)ts(190), build_sdlcolor(0x00000000));
        showText("2", ts(16), (int)(10 / 3 * dist - 2 * offset), (int)ts(190), build_sdlcolor(0x00000000));
    }
    for (auto i = 0u; i < numOfplayer; i++) {
        winnersTextures_[numOfplayer - i - 1]->render({ (int)(i * dist + offset), (int)ts(200), (int)ts(50), (int)ts(50) });

        if (!fmngr->getTeammode())showText(to_string(i + 1), ts(16), (int)(i * dist + offset), (int)ts(190), build_sdlcolor(0x00000000));

        showText("Kills: ", ts(6), (int)(i * dist + offset + ts(0)), (int)ts(255), build_sdlcolor(0x00000000));
        showText(to_string(gameStats_[numOfplayer - i - 1][2]), ts(6), (int)(i * dist + offset + ts(100)), (int)ts(255), build_sdlcolor(0x00000000));

        showText("Deaths: ", ts(6), (int)(i * dist + offset + ts(0)), (int)ts(265), build_sdlcolor(0x00000000));
        showText(to_string(gameStats_[numOfplayer - i - 1][0]), ts(6), (int)(i * dist + offset + ts(100)), (int)ts(265), build_sdlcolor(0x00000000));

        showText("Damage taken: ", ts(6), (int)(i * dist + offset + ts(0)), (int)ts(275), build_sdlcolor(0x00000000));
        showText(to_string(gameStats_[numOfplayer - i - 1][1]), ts(6), (int)(i * dist + offset + ts(100)), (int)ts(275), build_sdlcolor(0x00000000));
    }
}