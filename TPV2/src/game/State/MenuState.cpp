#include "MenuState.h"
#include "PlayingState.h"
#include "ConfigState.h"
#include "../PlayingState/FightManager.h"


MenuState::MenuState(FightManager* game) : State(game) {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background = &sdl->images().at("menu");
    exit = new Button( &sdl->images().at("ExitBut"),0,h -ts(21),ts(40),ts(20));
    sdl->musics().at("main").play();
    config = new Button(&sdl->images().at("ConfigBut"), w-ts(20), h - ts(21), ts(20), ts(20));
}

void MenuState::update() {
    for (auto i = 0u; i < SDL_NumJoysticks(); i++) {
        if (ih.xboxGetAxesState(i, 1) == -1) {
            std::cout << i << std::endl;
            Finput = i;
            fmngr->getState()->next();
            return;
        }
    }
    if (ih.isKeyDown(SDLK_w) && ih.keyDownEvent()) {
        Finput = -1;
        fmngr->getState()->next();
        return;
    }
    if (ih.isKeyDown(SDLK_UP) && ih.keyDownEvent()) {
        Finput = -2;
        fmngr->getState()->next();
        return;
    }
    
    if (textTimer + 800 < SDL_GetTicks() ) {
        textTimer = SDL_GetTicks();
        drawText = !drawText;
    }
    if (config->mouseClick()) {
        if (fmngr->getSavedState() == nullptr) {
            //pause
            std::cout << "pause" << std::endl;
            fmngr->saveState(fmngr->getState());
            fmngr->setState(new ConfigurationState(fmngr));
            return;
        }
        else
        {
            State* tmp = fmngr->getState();
            State* saved = fmngr->getSavedState();
            fmngr->setState(saved);
            fmngr->saveState(tmp);
        }
    }
    if (exit->mouseClick()) fmngr->userExit();
    if (ih.isKeyDown(SDLK_ESCAPE))
        fmngr->userExit();
}

void MenuState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    if (drawText)
        showText(" Pulse Flecha Arriba o la Tecla W o Joystick Arriba en su mando", ts(7) , ts(40), ts(220), build_sdlcolor(0x112233ff));
    exit->render();
    config->render();
    sdl->presentRenderer();  
}

void MenuState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new ConfigState(fmngr,Finput));
    delete this;
}
