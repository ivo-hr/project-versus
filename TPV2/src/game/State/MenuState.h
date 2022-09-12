#pragma once

#include "State.h"

class MenuState : public State {
    Texture* background;
    short Finput = -3;
    Uint32 textTimer = 0;
    bool drawText = false;
    Button* exit ,*config;
public:

    MenuState(FightManager* game);
    virtual ~MenuState();

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Menu state";
    };
};
