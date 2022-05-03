#pragma once

#include "State.h"

class ExitState : public State {
    Button *yes, * no;
public:

    ExitState(FightManager* fmngr);
    virtual  ~ExitState();

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Pause state";
    };
};