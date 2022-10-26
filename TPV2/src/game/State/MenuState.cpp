#include "MenuState.h"
#include "PlayingState.h"
#include "ExitState.h"
#include "ConfigState.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"
#include "../Utils/PlayerConfigs.h"


MenuState::MenuState(FightManager* game) : State(game) {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background = &sdl->images().at("menu");
    exit = new Button( &sdl->images().at("ExitBut"), 0, h * 11 / 12, w / 12, h / 12);
    sdl->musics().at("main").play();
    config = new Button(&sdl->images().at("ConfigBut"), w * 15 / 16, h - w / 16, w / 16, w / 16);

    SDL_ShowCursor(1);
}

MenuState::~MenuState()
{
    delete exit;
    delete config;
}

void MenuState::update() {
    for (auto i = 0; i < SDL_NumJoysticks(); i++) {
        if (ih.xboxGetAxesState(i, 1) == -1) {
            std::cout << i << std::endl;
            Finput = i;
            fmngr->getState()->next();
            return;
        }
    }
    if (ih.isKeyDown(playerPrefs.Keyboard1Up()) && ih.keyDownEvent()) {
        Finput = -1;
        fmngr->getState()->next();
        return;
    }
    if (ih.isKeyDown(playerPrefs.Keyboard2Up()) && ih.keyDownEvent()) {
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
            fmngr->setState(new ConfigurationState(fmngr,-1));
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

    if (ih.isKeyDown(SDLK_ESCAPE) && ih.keyDownEvent()) {
        if (fmngr->getExitState() == nullptr) {
            //pause
            fmngr->saveExitState(fmngr->getState());
            fmngr->setState(new ExitState(fmngr));
            return;
        }
    }
}

void MenuState::draw() {
    ushort w = fmngr->GetActualWidth();
    ushort h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,w,h });
    if (drawText)
        showText("PRESS ARROW UP / JOYSTICK UP / W TO START", h / 42 , w / 2 - (h / 42 * 20), h * 3 / 4 - h / 42, build_sdlcolor(0xFFFFFFff));
    exit->render();
    config->render();
    sdl->presentRenderer();  
}

void MenuState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new ConfigState(fmngr, Finput));

    sdl->soundEffects().at("uiSelect").play();
    delete this;
}
