#include "State.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"

bool State::doQuit() {
    return false;
}

void State::showText(string s, int size, int x, int y, SDL_Color c)
{
    string fontstring = "nes" + to_string(size);
    auto& font = sdl->fonts().at(fontstring);
    string key = fontstring + s + to_string(c.r) + to_string(c.g) + to_string(c.b);
    if (sdl->msgs().count(key) == 0) {
        sdl->msgs().emplace(key, Texture(sdl->renderer(), s, font, c));
    }
    tex = &sdl->msgs().at(key);
    tex->render(x, y);
}

void State::showText(string s, int size, int x, int y, SDL_Color c, SDL_Color b)
{
    if (size <= 16) {
        auto& font = sdl->fonts().at("nes16");
        tex = new Texture(sdl->renderer(), s, font, c,b);
    }
    else if (size > 16 && size<=24) {
        auto& font = sdl->fonts().at("nes24");
        tex = new Texture(sdl->renderer(), s, font, c,b);
    }
    else if (size > 24) {
        auto& font = sdl->fonts().at("nes48");
        tex = new Texture(sdl->renderer(), s, font, c,b);
    }

    tex->render(x, y);
}

void State::jump(State* state) {
    fmngr->setState(state);
    delete this;
}

void State::jumpWithoutDelete(State* state)
{
    fmngr->setState(state);
}

