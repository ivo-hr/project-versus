#include "ExitState.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"
#include "../Utils/PlayerConfigs.h"
#include <iostream>
#include <vector>
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Utils/Button.h"
#include "../Utils/PlayerConfigs.h"

using namespace std;

ExitState::ExitState(FightManager* game) : State(game) {

    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();

    auto aux = vector<PlayerPointer*>();

    string fontstring = "nes" + to_string((int)h / 42);
    auto& font = sdl->fonts().at(fontstring);
    SDL_Color c = build_sdlcolor(0xFF333300);
    string key = fontstring + "YES" + to_string(c.r) + to_string(c.g) + to_string(c.b);
    if (sdl->msgs().count(key) == 0) {
        sdl->msgs().emplace(key, Texture(sdl->renderer(), "YES", font, c));
    }
    yes = new Button(&sdl->msgs().at(key), w / 2 + w / 20, (h * 2 / 3) - w / 20, w / 20, w / 20, aux);
    yes->SetOnClick([this]()
        {
            fmngr->userExit();
        });
    yes->SetNoPointers();

    c = build_sdlcolor(0x33FFFC00);
    key = fontstring + "NO" + to_string(c.r) + to_string(c.g) + to_string(c.b);
    if (sdl->msgs().count(key) == 0) {
        sdl->msgs().emplace(key, Texture(sdl->renderer(), "NO", font, c));
    }
    no = new Button(&sdl->msgs().at(key), w / 2 - w / 10, (h * 2 / 3) - w / 20, w / 20, w / 20, aux);
    no->SetOnClick([this]()
        {
            fmngr->loadSavedState();
        });
    no->SetNoPointers();

    SDL_ShowCursor(1);
}

ExitState::~ExitState()
{
    delete yes;
    delete no;
}

void ExitState::update() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    if (ih->isKeyDown(SDLK_ESCAPE) && ih->keyDownEvent()) {
        fmngr->loadSavedState();
        return;
    }

    int i1 = (w / 3);
    int i2 = (w * 2 / 3);
    if (ih->getMousePos().first < i1 || ih->getMousePos().first > i2 || ih->getMousePos().second < (h / 3) || ih->getMousePos().second >(h * 2 / 3)) {
        if (ih->getMouseButtonState(ih->LEFT)) {
            fmngr->loadSavedState();
            return;
        }
    }

    yes->update();
    no->update();
}

void ExitState::draw() 
{
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();

    Texture* p = &sdl->images().at("Exitbg");
    p->render({(int)(w * 0.3f),(int)(h * 0.3f),(int)(w * 0.4f),(int)(h * 0.4f) });

    showText("DO YOU WANNA EXIT THE GAME?", (ushort)(w * 0.0145f), (ushort)(w * 0.311f), (ushort)(h * 0.32f), 0x33FFFC00);

    yes->render();
    no->render();
    sdl->presentRenderer();

}

void ExitState::next() {
    delete this;
}