#pragma once

#include "State.h"

class ConfigState : public State {
    Texture* background;
    int numOfplayer;
    Button* plus, minus;
public:

    ConfigState(FightManager* game) ;
    ~ConfigState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Config state";
    };
};