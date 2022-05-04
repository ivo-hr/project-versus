#include "ExitState.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"

ExitState::ExitState(FightManager* game) : State(game) {

    string fontstring = "nes" + to_string((int)ts(8));
    auto& font = sdl->fonts().at(fontstring);
    SDL_Color c = build_sdlcolor(0xFF333300);
    string key = fontstring + "YES" + to_string(c.r) + to_string(c.g) + to_string(c.b);
    if (sdl->msgs().count(key) == 0) {
        sdl->msgs().emplace(key, Texture(sdl->renderer(), "YES", font, c));
    }
    yes = new Button(&sdl->msgs().at(key), ts(290), ts(150), ts(30), ts(30));

    c = build_sdlcolor(0x33FFFC00);
    key = fontstring + "NO" + to_string(c.r) + to_string(c.g) + to_string(c.b);
    if (sdl->msgs().count(key) == 0) {
        sdl->msgs().emplace(key, Texture(sdl->renderer(), "NO", font, c));
    }
    no = new Button(&sdl->msgs().at(key), ts(190), ts(150), ts(30), ts(30));
}

ExitState::~ExitState()
{
    delete yes;
    delete no;
}



void ExitState::update() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    if (ih.isKeyDown(SDLK_ESCAPE) && ih.keyDownEvent() || no->mouseClick()) {
        std::cout << "unpause" << std::endl;
        State* tmp = fmngr->getState();
        State* saved = fmngr->getExitState();
        fmngr->setState(saved);
        fmngr->clearExitState();
        delete tmp;
        return;
    }
    if (ih.getMousePos().first < (w / 2 - ts(125)) || ih.getMousePos().first >(w / 2 - ts(125))) {
        if (ih.getMousePos().second < (h / 2 - ts(50)) || ih.getMousePos().second >(h / 2 + ts(50))) {
            if (ih.getMouseButtonState(ih.LEFT)) {
                State* tmp = fmngr->getState();
                State* saved = fmngr->getExitState();
                fmngr->setState(saved);
                fmngr->clearExitState();
                delete tmp;
                return;
            }
        }
    }
    int i1 = (w / 2 - ts(125));
    int i2 = (w / 2 + ts(125));
    if (ih.getMousePos().first < i1 || ih.getMousePos().first > i2) {
        if (ih.getMouseButtonState(ih.LEFT)) {
            State* tmp = fmngr->getState();
            State* saved = fmngr->getExitState();
            fmngr->setState(saved);
            fmngr->clearExitState();
            delete tmp;
            return;
        }
       
    }
    
    if (yes->mouseClick()) {
        fmngr->userExit();
        State* saved = fmngr->getExitState();
        fmngr->setState(saved);
        delete this;
    }
}

void ExitState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    Texture* p = &sdl->images().at("Exitbg");
    p->render({(int)(w/2-ts(125)),(int)(h/2-ts(50)),(int)ts(250),(int)ts(100) });
    showText("DO U WANNA EXIT THE GAME", ts(8), ts(160), ts(100), build_sdlcolor(0x33FFFC00));
    yes->render();
    no->render();
    sdl->presentRenderer();

}

void ExitState::next() {
    cout << "Next State " << endl;
    delete this;
}