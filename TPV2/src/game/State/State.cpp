#include "State.h"
#include "../PlayingState/FightManager.h"

bool State::doQuit() {
    return false;
}

void State::showText(string s, int size, int x, int y, SDL_Color c)
{
    string fontstring = "nes" + to_string(size);

    auto& font = sdl->fonts().at(fontstring);
    tex = new Texture(sdl->renderer(), s, font, c);
    
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

double State::ts(double i)
{
    return i * fmngr->GetScreeAdjust();
}

void State::jump(State* state) {
    fmngr->setState(state);
    delete tex;
    delete this;
}

