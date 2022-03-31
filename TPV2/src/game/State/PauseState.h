#pragma once

#include "State.h"

class PauseState : public State {

public:

    PauseState(FightManager* fmngr);
    ~PauseState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Pause state";
    };
};