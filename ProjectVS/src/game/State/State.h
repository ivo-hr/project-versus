#pragma once
#include <iostream>
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Utils/Button.h"
#include "../Utils/PlayerConfigs.h"

class FightManager;

using ushort = unsigned short;
using uchar = unsigned char;

using namespace std;

class State {
protected:
    PlayerConfigs& playerPrefs = *PlayerConfigs::instance();
    FightManager* fmngr = nullptr;
    InputHandler& ih = *InputHandler::instance();
    SDLUtils* sdl = SDLUtils::instance();
    void showText(string s, ushort size, ushort x, ushort y,SDL_Color c);
    void showText(string s, ushort size, ushort x, ushort y, SDL_Color c, SDL_Color b);
    //double ts(double i);//TO SCREEN
    Texture* tex;
    vector<short> globalplayerInput;
public:
    State(FightManager* fmngr) : fmngr(fmngr) ,tex(nullptr) {
    };

    virtual ~State() {    
    };
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void next() = 0;
    virtual void OnEnable() = 0;
    virtual void OnDisable() = 0;
    virtual void Reset() = 0;
    virtual void jump(State* state);
    virtual void jumpWithoutDelete(State* state);
    bool doQuit();
    virtual string getStateName() const = 0;
};