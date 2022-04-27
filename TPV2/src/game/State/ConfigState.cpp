#include "ConfigState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"




ConfigState::ConfigState(FightManager* game , int fInput) : State(game), numOfplayer(2) {
    background = &sdl->images().at("selectbg");
    aleatorio = nullptr;
    nasnas = nullptr;
    zero = new Button(&sdl->images().at("zero"), ts(30), ts(50), ts(30), ts(30));
    gatoespia = new Button(&sdl->images().at("blinkMaster"), ts(90), ts(50), ts(30), ts(30));
    maketo = new Button(&sdl->images().at("makt"), ts(150), ts(50), ts(30), ts(30));
    togo = new Button(&sdl->images().at("dinoSouls"), ts(210), ts(50), ts(30), ts(30));
    plusB = new Button(&sdl->images().at("pB"), ts(490), ts(210), ts(20), ts(30));
    minusB = new Button(&sdl->images().at("mB"), ts(490), ts(240), ts(20), ts(30));
   
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P1")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P2")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P3")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P4")));

    usedKeyboard.resize(2);
    playerInput.resize(1);
    playerInput[0] = fInput;
    playerTexture[0]->setgotInput(true);
    charactersSelect.resize(2);
    usedPad.resize(SDL_NumJoysticks());


    if (fInput >= 0) { usedPad[fInput] = true; playerTexture[0]->setFront(&sdl->images().at("Mando")); }
    else if (fInput == -1) { usedKeyboard[0] = true; playerTexture[0]->setFront(&sdl->images().at("k1"));
    }
    else if (fInput == -2) { usedKeyboard[1] = true; playerTexture[0]->setFront(&sdl->images().at("k2"));
    }
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P1P"), 0, 0, ts(10), ts(10),w,h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P2P"), 0, 0, ts(10), ts(10),w,h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P3P"), 0, 0, ts(10), ts(10), w, h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P4P"), 0, 0, ts(10), ts(10), w, h));
    playerPointers[0]->setActive(true);
    sdl->musics().at("sawtines").play();
}

ConfigState::~ConfigState()
{
    delete zero;
    delete gatoespia;
    delete maketo;
    delete togo;
    delete nasnas;
    delete aleatorio;
    delete plusB;
    delete minusB;
    for (auto e : playerPointers)delete e;
    for (auto e : playerTexture)delete e;
    for (auto e : charactTexture)delete e;
}

void ConfigState::update() {
    //Si hay algun input pendiente , buscarlo
    if (playerInput.size()<numOfplayer) {
        for (auto i = 0u; i < SDL_NumJoysticks(); i++) {
            if (ih.xboxGetAxesState(i, 1) == -1 && !usedPad[i]) {
                usedPad[i] = true;
                playerInput.push_back(i);
                playerPointers[playerInput.size() - 1]->setActive(true);
                playerTexture[playerInput.size() - 1]->setgotInput(true);
                playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("Mando"));
                charactersSelect.resize(playerInput.size());
                return;
            }
        }
        if (ih.isKeyDown(SDLK_UP) && !usedKeyboard[1]) {
            usedKeyboard[1] = true;
            playerInput.push_back(-2);
            playerPointers[playerInput.size() - 1]->setActive(true);
            playerTexture[playerInput.size() - 1]->setgotInput(true);
            playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("k2"));
        }
        if (ih.isKeyDown(SDLK_w)&& !usedKeyboard[0]) {
            usedKeyboard[0] = true;
            playerInput.push_back(-1);
            playerPointers[playerInput.size() - 1]->setActive(true);
            playerTexture[playerInput.size() - 1]->setgotInput(true);
            playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("k1"));
        }
        charactersSelect.resize(playerInput.size());
    }
    //Movimiento de los punteros
    for (auto i = 0; i < playerInput.size(); i++) {
        switch (playerInput[i])
        {
        case -1:
            if (ih.isKeyDown(SDLK_w))playerPointers[i]->move(0);
            if (ih.isKeyDown(SDLK_s))playerPointers[i]->move(1);
            if (ih.isKeyDown(SDLK_a))playerPointers[i]->move(2);
            if (ih.isKeyDown(SDLK_d))playerPointers[i]->move(3);
            break;
        case -2:
            if (ih.isKeyDown(SDLK_UP))playerPointers[i]->move(0);
            if (ih.isKeyDown(SDLK_DOWN))playerPointers[i]->move(1);
            if (ih.isKeyDown(SDLK_LEFT))playerPointers[i]->move(2);
            if (ih.isKeyDown(SDLK_RIGHT))playerPointers[i]->move(3);
            break;
        default:
            if(ih.xboxGetAxesState(playerInput[i], 1) == -1 || ih.xboxGetDpadState(playerInput[i], 0))playerPointers[i]->move(0);
            if(ih.xboxGetAxesState(playerInput[i], 1) == 1 || ih.xboxGetDpadState(playerInput[i], 2))playerPointers[i]->move(1);
            if(ih.xboxGetAxesState(playerInput[i], 0) == -1 || ih.xboxGetDpadState(playerInput[i] ,3))playerPointers[i]->move(2);
            if(ih.xboxGetAxesState(playerInput[i], 0) == 1 || ih.xboxGetDpadState(playerInput[i], 1))playerPointers[i]->move(3);
            break;
        }
    }
    //Comprobacion de punteros con los botones
    for (auto i = 0; i < playerInput.size(); i++) {
        bool enter = false;
        switch (playerInput[i])
        {
        case -1:
            if (ih.isKeyDown(SDLK_e))enter = true;
            break;
        case -2:
            if (ih.isKeyDown(SDLK_l))enter = true;
            break;
        default:
            if( ih.xboxGetButtonState(playerInput[i], SDL_CONTROLLER_BUTTON_A))enter = true;
            break;
        }
        if (zero->pointerClick(playerPointers[i]->getRect())&&enter && keyRelease) {
            playerTexture[i]->setFront(&sdl->images().at("zero"));
            charactersSelect[i] = 0;
            keyRelease = false;
        }
        else if (gatoespia->pointerClick(playerPointers[i]->getRect()) && enter) {
            playerTexture[i]->setFront(&sdl->images().at("blinkMaster"));
            charactersSelect[i] = 1;
            keyRelease = false;
        }
        else if (togo->pointerClick(playerPointers[i]->getRect()) && enter) {
            playerTexture[i]->setFront(&sdl->images().at("dinoSouls"));
            charactersSelect[i] = 2;
            keyRelease = false;
        }
        else if (maketo->pointerClick(playerPointers[i]->getRect()) && enter) {
            playerTexture[i]->setFront(&sdl->images().at("makt"));
            charactersSelect[i] = 3;
            keyRelease = false;
        }
        else if (minusB->pointerClick(playerPointers[i]->getRect()) && enter && numOfplayer > 2 && keyRelease) {
            numOfplayer--;

            if (playerInput.size() > numOfplayer) {
                switch (playerInput[playerInput.size() - 1])
                {
                case -1:
                    usedKeyboard[0] = false;
                    break;
                case -2:
                    usedKeyboard[1] = false;
                    break;
                default:
                    usedPad[playerInput[playerInput.size() - 1]] = false;
                    break;
                }
                playerInput.resize(numOfplayer);
                playerPointers[playerInput.size()]->setActive(false);
                playerTexture[playerInput.size()]->setgotInput(false);
            }
            charactersSelect.resize(playerInput.size());
            keyRelease = false;
        }
        else if (plusB->pointerClick(playerPointers[i]->getRect()) && enter && numOfplayer < 4 && keyRelease) {
            numOfplayer++;
            charactersSelect.resize(playerInput.size());
            keyRelease = false;
        }
        switch (playerInput[i])
        {
        case -1:
            if (!ih.isKeyDown(SDLK_e))keyRelease = true;
            break;
        case -2:
            if (!ih.isKeyDown(SDLK_l))keyRelease = true;
            break;
        default:
            if (!ih.xboxGetButtonState(playerInput[i], SDL_CONTROLLER_BUTTON_A))keyRelease = true;
            break;
        }
    }
    //Para +- numPlayer con el raton
    if (minusB->mouseClick()  && numOfplayer > 2) {
        numOfplayer--;

        if (playerInput.size()>numOfplayer) {
            switch (playerInput[playerInput.size() - 1])
            {
            case -1:
                usedKeyboard[0] = false;
                break;
            case -2:
                usedKeyboard[1] = false;
                break;
            default:
                usedPad[playerInput[playerInput.size() - 1]] = false;
                break;
            }
            playerInput.resize(numOfplayer);
            playerPointers[playerInput.size()]->setActive(false);
            playerTexture[playerInput.size()]->setgotInput(false);
        }
        charactersSelect.resize(playerInput.size());
    }
    else if (plusB->mouseClick()  && numOfplayer < 4) {
        numOfplayer++;
        charactersSelect.resize(playerInput.size());
    }
    //Empezar la partida
    if (ih.isKeyDown(SDLK_RETURN))fmngr->getState()->next();
}

void ConfigState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });

    for (auto i = 0u; i < numOfplayer; i++) {
        int dist = (w-ts(50)) / numOfplayer;
        int offset = dist - ts(110);
        playerTexture[i]->render((int)(i*dist + offset), (int)ts(200), (int)ts(110), (int)ts(80));
    }
    zero->render();
    gatoespia->render();
    togo->render();
    maketo->render();
    plusB->render();
    minusB->render();
    for (auto e : playerPointers)e->render();
    sdl->presentRenderer();
}

void ConfigState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new PlayingState(fmngr, playerInput, charactersSelect));
    delete this;
}