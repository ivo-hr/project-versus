#include "ConfigState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"


double ConfigState::ts(double i) //TO SCREEN
{
    return i * fmngr->GetScreeAdjust();
}

ConfigState::ConfigState(FightManager* game) : State(game), numOfplayer(2) {
    background = &sdl->images().at("fondo");
    exp = &sdl->images().at("exp");
    keyb = new Button(&sdl->images().at("fondo"), 0, 0, ts(30), ts(30));
    nes = new Button(&sdl->images().at("pause"), 0, 0, ts(30), ts(30));
    xbox = new Button(&sdl->images().at("star"), 0, 0, ts(30), ts(30));
    play = new Button(&sdl->images().at("play"),ts(400), ts(250), ts(60), ts(30));
    nextb = new Button(&sdl->images().at("next"), ts(400), ts(250), ts(60), ts(30));
    back = new Button(&sdl->images().at("back"), ts(20), ts(250), ts(60), ts(30));
    player.resize(2);
    charact.resize(2);
}

void ConfigState::update() {
    
     if (ih.isKeyDown(SDLK_LEFT) && ih.keyDownEvent() && numOfplayer > 2) {
        sel = 0;
        numOfplayer--;
        player.resize(numOfplayer);
        charact.resize(numOfplayer);
        play->setUnrendered();
    }
    else if (ih.isKeyDown(SDLK_RIGHT) && ih.keyDownEvent() && numOfplayer < 4) {
        sel = 0;
        numOfplayer++;
        player.resize(numOfplayer);
        charact.resize(numOfplayer);
        play->setUnrendered();
    }
    if (keyb->mouseClick()) {
        if (!charsel)
            player[sel] = 0;
        else
            charact[sel] = 0;
        if(sel<numOfplayer)
        sel++;
    }
    else if(nes->mouseClick())
    {
        if (!charsel)
            player[sel] = 1;
        else
            charact[sel] = 1;
        if (sel < numOfplayer)
        sel++;
    }
    else if (xbox->mouseClick()) {
        if(!charsel)
        player[sel] = 2;
        else
        charact[sel] = 2;
        if (sel < numOfplayer)
        sel++;
    }
   
    if (nextb->mouseClick()) {
        charsel = true;
        sel = 0;
    }
    if (back->mouseClick()){
        charsel = false;
        sel = 0;
    }
    if (play->mouseClick())fmngr->getState()->next();
}

void ConfigState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    exp->render({w / 2 - (int)ts(100)/2 ,h- h / 4,(int) ts(100),(int)ts(50) });

    if(!charsel)
    showText("InputConfig", ts(8), w / 2 - ts(25), ts(5), build_sdlcolor(0x112233ff),build_sdlcolor(0xffffffff));
    else
    showText("CharConfig", ts(8), w / 2 - ts(25) , ts(5), build_sdlcolor(0x112233ff),build_sdlcolor(0xffffffff));
    for (auto i = 0u; i < numOfplayer; i++) {
        int wOFF = ts(100);
        int hOFF = ts(50);
        showText(" Player "+ to_string(i+1) , ts(8) , wOFF, h/2- wOFF +i* hOFF, build_sdlcolor(0x112233ff));
        if (i == sel) {
            keyb->setX(wOFF + hOFF+ts(5)); keyb->setY(h / 2 - wOFF + i * hOFF);
            nes->setX(wOFF + hOFF*2 + ts(5)); nes->setY(h / 2 - wOFF + i * hOFF);
            xbox->setX(wOFF + hOFF*3 + ts(5)); xbox->setY(h / 2 - wOFF + i * hOFF);
            keyb->render();
            nes->render();
            xbox->render();
        }
    }
    if (sel == numOfplayer) {
        if(charsel)play->render();
        else {
            nextb->render();
            back->render();
        }
    }
    sdl->presentRenderer();
}

void ConfigState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new PlayingState(fmngr,player,charact));
    delete this;
}
