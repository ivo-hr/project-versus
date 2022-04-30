#pragma once

#include "State.h"

class ConfigurationState : public State {
    Uint32 startTime;
public:

    ConfigurationState(FightManager* fmngr);
    ~ConfigurationState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Pause state";
    };
};