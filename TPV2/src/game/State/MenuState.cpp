#include "MenuState.h"
#include "PlayingState.h"
#include "ConfigState.h"
#include "../PlayingState/FightManager.h"


MenuState::MenuState(FightManager* game) : State(game) {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background = &sdl->images().at("menu");

    sdl->musics().at("main").play();
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
}

void MenuState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    if (drawText)
        showText(" Pulse Flecha Arriba o la Tecla W o Joystick Arriba en su mando", ts(8), ts(80), ts(220), build_sdlcolor(0x112233ff));
    sdl->presentRenderer();  
}

void MenuState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new ConfigState(fmngr,Finput));
    delete this;
}
