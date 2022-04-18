#pragma once

#include "State.h"

class PlayingState : public State {

public:

    PlayingState(FightManager* fmngr , vector<int>player, vector<int>characters);
    ~PlayingState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Playing state";
    };
};