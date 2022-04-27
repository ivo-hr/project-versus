#include "ConfigState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"




ConfigState::ConfigState(FightManager* game , int fInput) : State(game), numOfplayer(2) {
    background = &sdl->images().at("selectbg");
    //keyb = new Button(&sdl->images().at("fondo"), 0, 0, ts(30), ts(30));
    //nes = new Button(&sdl->images().at("pause"), 0, 0, ts(30), ts(30));
    //xbox = new Button(&sdl->images().at("star"), 0, 0, ts(30), ts(30));
    //play = new Button(&sdl->images().at("play"),ts(400), ts(250), ts(60), ts(30));
    //nextb = new Button(&sdl->images().at("next"), ts(400), ts(250), ts(60), ts(30));
    //back = new Button(&sdl->images().at("back"), ts(20), ts(250), ts(60), ts(30));
    zero = new Button(&sdl->images().at("zero"), ts(30), ts(50), ts(30), ts(30));
    gatoespia = new Button(&sdl->images().at("blinkMaster"), ts(90), ts(50), ts(30), ts(30));
    maketo = new Button(&sdl->images().at("makt"), ts(150), ts(50), ts(30), ts(30));
    togo = new Button(&sdl->images().at("dinoSouls"), ts(210), ts(50), ts(30), ts(30));

   
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

void ConfigState::update() {
    
    if (playerInput.size()<numOfplayer) {
        for (auto i = 0u; i < SDL_NumJoysticks(); i++) {
            if (ih.xboxGetAxesState(i, 1) == -1 && !usedPad[i]) {
                usedPad[i] = true;
                playerInput.push_back(i);
                playerPointers[playerInput.size() - 1]->setActive(true);
                playerTexture[playerInput.size() - 1]->setgotInput(true);
                playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("Mando"));
           /*     searchGamepad = false;
                if (sel < numOfplayer && !searchGamepad)
                    sel++;
                return;*/
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
    }

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
            if( ih.xboxGetButtonState(i, SDL_CONTROLLER_BUTTON_A))enter = true;
            break;
        }
        if (zero->pointerClick(playerPointers[i]->getRect())&&enter) {
            playerTexture[i]->setFront(&sdl->images().at("zero"));
            charactersSelect[i] = 0;
        }
        if (gatoespia->pointerClick(playerPointers[i]->getRect()) && enter) {
            playerTexture[i]->setFront(&sdl->images().at("blinkMaster"));
            charactersSelect[i] = 1;
        }
        if (togo->pointerClick(playerPointers[i]->getRect()) && enter) {
            playerTexture[i]->setFront(&sdl->images().at("dinoSouls"));
            charactersSelect[i] = 2;
        }
        if (maketo->pointerClick(playerPointers[i]->getRect()) && enter) {
            playerTexture[i]->setFront(&sdl->images().at("makt"));
            charactersSelect[i] = 3;
        }
    }
    if(ih.isKeyDown(SDLK_RETURN))fmngr->getState()->next();

    //if (ih.isKeyDown(SDLK_LEFT) && ih.keyDownEvent() && numOfplayer > 2) {
    //    sel = 0;
    //    numOfplayer--;

    //    if(numOfplayer > playerInput.size())
    //    switch (playerInput[playerInput.size()-1])
    //    {
    //    case -1:
    //        usedKeyboard[0] = false;
    //        break;
    //    case -2:
    //        usedKeyboard[1] = false;
    //        break;
    //    default:
    //        usedPad[playerInput[playerInput.size()-1]] = false;
    //        break;
    //    }
    //   
    //    playerInput.resize(numOfplayer);
    //    playerPointers[playerInput.size()]->setActive(false);

    //    //usedPad.clear();
    //    //usedPad.resize(SDL_NumJoysticks());

    //    charactersSelect.resize(numOfplayer);
    //}
    //else if (ih.isKeyDown(SDLK_RIGHT) && ih.keyDownEvent() && numOfplayer < 4) {
    //    sel = 0;
    //    numOfplayer++;
    //    charactersSelect.resize(numOfplayer);
    //}



    //if (keyb->mouseClick()) {
    //    if (!charsel && k >= -2) {
    //        player[sel] = k;
    //        k--;
    //    }
    //    else
    //        charact[sel] = 0;
    //    if (sel < numOfplayer)
    //        sel++;
    //}
    //else if(nes->mouseClick())
    //{
    //    if (!charsel) {
    //        searchGamepad = true;
    //    }
    //    else
    //        charact[sel] = 1;
    //    if (sel < numOfplayer && !searchGamepad) {
    //        sel++;
    //    }
    //}
    //else if (xbox->mouseClick()) {
    //    if(!charsel)
    //        searchGamepad = true;
    //    else
    //    charact[sel] = 2;
    //    if (sel < numOfplayer && !searchGamepad)
    //    sel++;
    //}
   
    //if (nextb->mouseClick()) {
    //    charsel = true;
    //    sel = 0;
    //}
    //if (back->mouseClick()){
    //    charsel = false;
    //    sel = 0;
    //}
    //if (play->mouseClick())fmngr->getState()->next();
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
        //int wOFF = ts(100);
        //int hOFF = ts(50);
        //showText(" Player "+ to_string(i+1) , ts(8) , wOFF, h/2- wOFF +i* hOFF, build_sdlcolor(0x112233ff));
        //if (i == sel) {
        //    keyb->setX(wOFF + hOFF+ts(5)); keyb->setY(h / 2 - wOFF + i * hOFF);
        //    nes->setX(wOFF + hOFF*2 + ts(5)); nes->setY(h / 2 - wOFF + i * hOFF);
        //    xbox->setX(wOFF + hOFF*3 + ts(5)); xbox->setY(h / 2 - wOFF + i * hOFF);
        //    keyb->render();
        //    nes->render();
        //    xbox->render();
        //}
    }
    zero->render();
    gatoespia->render();
    togo->render();
    maketo->render();
    //if (sel == numOfplayer) {
    //    if(charsel)play->render();
    //    else {
    //      /*  nextb->render();
    //        back->render();*/
    //    }
    //}
    if (searchGamepad)
        showText("Pulsa Joystick arriba en el mando que vas a usar", ts(8), w / 4, ts(50), build_sdlcolor(0x112233ff), build_sdlcolor(0xffffffff));
    for (auto e : playerPointers)e->render();
    sdl->presentRenderer();
}

void ConfigState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new PlayingState(fmngr, playerInput, charactersSelect));
    delete this;
}