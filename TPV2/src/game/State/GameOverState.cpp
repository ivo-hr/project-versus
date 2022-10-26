#include "GameOverState.h"
#include "MenuState.h"
#include "ExitState.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"
#include "../Utils/PlayerConfigs.h"


GameOverState::GameOverState(FightManager* game, vector<Texture*>winnersTextures, vector<vector<ushort>>gameStats, short playersInput, vector<char>playersInputV) : State(game) {

    //ts(15) = w / 64

    background = &sdl->images().at("gameoverscreen1");
    //fmngr = game;
    winnersTextures_ = winnersTextures;

    ushort w = fmngr->GetActualWidth();
    ushort h = fmngr->GetActualHeight();

    playAgain = new Button(&sdl->images().at("playagain"), w / 3, h / 4, w/3, h/4);

    int winnerInput=1;
    for (auto i = 0u; i < playersInputV.size(); i++) {
        if (playersInputV[i] == playersInput)winnerInput = i+1;
    }
    string inputString = "P" + to_string(winnerInput) + "P";
    pointer = new PlayerPointer(&sdl->images().at(inputString), w / 2 - w / 64 / 2, h / 2 - w / 64 / 2, w, h);
    pointer->setActive(true);
    playersInput_ = playersInput;
    gameStats_ = gameStats;

    sdl->musics().at("win").play();

    SDL_ShowCursor(1);
}

GameOverState::~GameOverState()
{
    delete playAgain;
    delete pointer;
}

void GameOverState::update() {

    if (playersInput_ < 0)
    {
        if (ih.isKeyDown(playerPrefs.KeyboardUp(playersInput_ == -1)))pointer->move(0);
        if (ih.isKeyDown(playerPrefs.KeyboardDown(playersInput_ == -1)))pointer->move(1);
        if (ih.isKeyDown(playerPrefs.KeyboardLeft(playersInput_ == -1)))pointer->move(2);
        if (ih.isKeyDown(playerPrefs.KeyboardRight(playersInput_ == -1)))pointer->move(3);
    }
    else
    {
        if (ih.xboxGetAxesState(playersInput_, 1) == -1 || ih.xboxGetDpadState(playersInput_, 0))pointer->move(0);
        if (ih.xboxGetAxesState(playersInput_, 1) == 1 || ih.xboxGetDpadState(playersInput_, 2))pointer->move(1);
        if (ih.xboxGetAxesState(playersInput_, 0) == -1 || ih.xboxGetDpadState(playersInput_, 3))pointer->move(2);
        if (ih.xboxGetAxesState(playersInput_, 0) == 1 || ih.xboxGetDpadState(playersInput_, 1))pointer->move(3);
    }

    bool enter = false;

    if (playersInput_ < 0)
    {
        if (ih.isKeyDown(playerPrefs.KeyboardBasic(playersInput_ == -1)))enter = true;
    }
    else
    {
        if (ih.xboxGetButtonState(playersInput_, playerPrefs.ControllerBasic()))enter = true;
    }

    if ((playAgain->pointerClick(pointer->getRect()) && enter) || playAgain->mouseClick()) {
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
    int numOfplayer = (int)gameStats_.size();
    int dist = w / numOfplayer;
    int offset = (w/2) / numOfplayer - w / 13;

    if (fmngr->getTeammode()) {
        showText("1", (h / 12), (int)(dist / 2 + 3 * offset), (int)(h * 2.5f / 5), build_sdlcolor(0x00000000));
        showText("2", (h / 12), (int)(10 / 3 * dist - 2 * offset), (int)(h * 2.5f / 5), build_sdlcolor(0x00000000));
    }
    for (auto i = 0; i < numOfplayer; i++) {
        winnersTextures_[numOfplayer - i - 1]->render({ (int)(i * dist + offset), (int)(h * 3 / 5), (int)w / 12, (int)w / 12 });

        if (!fmngr->getTeammode())
        {
            SDL_Color col;
            if (i == 0) { col = build_sdlcolor(0xEAD90A00); }
            else if (i == 1) { col = build_sdlcolor(0xC8CACA00); }
            else if (i == 2) { col = build_sdlcolor(0xA77A1900); }
            else { col = build_sdlcolor(0x69696900); }
            showText(to_string(i + 1), (h / 12), (int)(i * dist + offset), (int)(h * 2.5f / 5), col);
        }

        showText("Kills: ", (h / 42), (ushort)(i * dist + offset), (ushort)((float)h * 3.9f / (float)5), build_sdlcolor(0x00000000));
        showText(to_string(gameStats_[numOfplayer - i - 1][2]), (h / 42), (ushort)(i * dist + offset + w / 6), (ushort)(h * 3.9f / 5), build_sdlcolor(0x00000000));

        showText("Deaths: ", (h / 42), (ushort)(i * dist + offset), (ushort)((float)h * 4.05f / (float)5), build_sdlcolor(0x00000000));
        showText(to_string(gameStats_[numOfplayer - i - 1][0]), (h / 42), (ushort)(i * dist + offset + w / 6), (ushort)(h * 4.05f / 5), build_sdlcolor(0x00000000));

        showText("Dmg taken: ", (h / 42), (ushort)(i * dist + offset), (ushort)((float)h * 4.2f / (float)5), build_sdlcolor(0x00000000));
        showText(to_string(gameStats_[numOfplayer - i - 1][1]), (h / 42), (ushort)(i * dist + offset + w / 6), (ushort)(h * 4.2f / 5), build_sdlcolor(0x00000000));
    }
}