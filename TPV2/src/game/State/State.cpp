#include "State.h"
#include "../PlayingState/FightManager.h"

bool State::doQuit() {
    return false;
}

void State::showText(string s, int size, int x, int y, SDL_Color c)
{
    if (size == 16) {
        auto& font = sdl->fonts().at("ARIAL16");
        tex = new Texture(sdl->renderer(), s, font, c);
    }
    else if (size == 24) {
        auto& font = sdl->fonts().at("ARIAL24");
        tex = new Texture(sdl->renderer(), s, font, c);
    }

    else if (size == 48) { auto& font = sdl->fonts().at("ARIAL48"); 
    tex = new Texture(sdl->renderer(), s, font, c);
    }
    
    tex->render(x, y);

}

void State::jump(State* state) {
    fmngr->setState(state);
    delete tex;
    delete this;
}

