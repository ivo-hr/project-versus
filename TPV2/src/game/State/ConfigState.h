#pragma once

#include "State.h"

class ConfigState : public State {
    Texture* background, *exp;
    int numOfplayer;
    Button* keyb, *nes, *xbox ,*play , *nextb ,*back;
    vector<int> player;
    vector<int> charact;
    int sel = 0;
    int key = 0;
    int nesn = 0;
    int xboxn = 0;
    bool charsel = false;
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