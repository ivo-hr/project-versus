#include "State.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"
#include <iostream>
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Utils/Button.h"
#include "../Utils/PlayerConfigs.h"

using namespace std;

bool State::doQuit() {
    return false;
}

void State::showText(string s, ushort size, ushort x, ushort y, unsigned int hexColor)
{
    SDL_Color c = build_sdlcolor(hexColor);
    string fontstring = "nes" + to_string(size);
    auto& font = sdl->fonts().at(fontstring);
    string key = fontstring + s + to_string(c.r) + to_string(c.g) + to_string(c.b);
    if (sdl->msgs().count(key) == 0) {
        sdl->msgs().emplace(key, Texture(sdl->renderer(), s, font, c));
    }
    tex = &sdl->msgs().at(key);
    tex->render(x, y);
}

State::State(FightManager* fmngr) : fmngr(fmngr), tex(nullptr) {
    playerPrefs = PlayerConfigs::instance();
    ih = InputHandler::instance();
    sdl = SDLUtils::instance();
}

void State::jump(State* state) {
    fmngr->setState(state);
    delete this;
}

void State::jumpWithoutDelete(State* state)
{
    fmngr->setState(state);
}

void State::Reset()
{

}