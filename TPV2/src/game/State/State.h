#pragma once
#include <iostream>
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Utils/Button.h"

class FightManager;
using namespace std;

class State {
protected:
    FightManager* fmngr = nullptr;
    InputHandler& ih = *InputHandler::instance();
    SDLUtils* sdl = SDLUtils::instance();
    void showText(string s, int size, int x,int y,SDL_Color c);
    void showText(string s, int size, int x, int y, SDL_Color c, SDL_Color b);
    double ts(double i);//TO SCREEN
    Texture* tex;
    vector<int> globalplayerInput;
public:
    State(FightManager* fmngr) : fmngr(fmngr) ,tex(nullptr) {
    };

    virtual ~State() {    
    };
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void next() = 0;
    virtual void jump(State* state);
    virtual void jumpWithoutDelete(State* state);
    bool doQuit();
    virtual string getStateName() const = 0;
};