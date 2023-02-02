#include "GameOverState.h"
#include "MenuState.h"
#include "ExitState.h"
#include "ConfigState.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"
#include "../Utils/PlayerConfigs.h"


GameOverState::GameOverState(FightManager* game, vector<Texture*>winnersTextures, vector<vector<ushort>>gameStats, short playersInput, vector<char>playersInputV) : 
    State(game), pointer(pointers[0])
{
    ushort w = fmngr->GetActualWidth();
    ushort h = fmngr->GetActualHeight();

    allInputs = playersInputV;

    int winnerInput = 1;
    for (auto i = 0u; i < playersInputV.size(); i++) {
        if (playersInputV[i] == playersInput)winnerInput = i + 1;
    }
    string inputString = "P" + to_string(winnerInput) + "P";
    pointers[0] = new PlayerPointer(&sdl->images().at(inputString), w / 2 - w / 64 / 2, h / 2 - w / 64 / 2, w, h, playersInput);

    background = &sdl->images().at("gameoverscreen1");
    //fmngr = game;
    winnersTextures_ = winnersTextures;

    playAgain = new Button(&sdl->images().at("playagain"), w / 3, h / 4, w/3, h/4, pointers);
    playAgain->SetOnClick([this]()
        {
            fmngr->getState()->next();
        });

    pointer = pointers[0];
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

void GameOverState::update()
{

    pointers[0]->update();
    playAgain->update();
    //if (ih.isKeyDown(SDLK_e))fmngr->getState()->next();
}

void GameOverState::draw() {
    sdl->clearRenderer();
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    drawGameStats();
    playAgain->render();
    pointer->render();
    sdl->presentRenderer();
}

void GameOverState::next() {
    fmngr->setState(new ConfigState(fmngr, allInputs));
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