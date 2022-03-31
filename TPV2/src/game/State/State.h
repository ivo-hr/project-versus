#pragma once
#include <iostream>
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"

class FightManager;
using namespace std;

class State {
protected:
    FightManager* fmngr = nullptr;
    InputHandler& ih = *InputHandler::instance();
public:
    State(FightManager* fmngr) : fmngr(fmngr) {
    };

    virtual ~State() {
    
    };
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void next() = 0;
    virtual void jump(State* state);
    bool doQuit();
    virtual string getStateName() const = 0;
};