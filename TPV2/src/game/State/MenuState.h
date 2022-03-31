#pragma once

#include "State.h"

class MenuState : public State {

public:

    MenuState(FightManager* game);
    ~MenuState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Menu state";
    };
};
