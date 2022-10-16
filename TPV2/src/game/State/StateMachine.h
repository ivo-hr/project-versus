#pragma once
#include "State.h"

class StateMachine {
protected:
    State* state = nullptr;
    State* savedState = nullptr;
    State* exitState = nullptr;
public:
    //~StateMachine() {
    //    if(state!=nullptr)
    //    delete state;
    //}
    void setState(State* state) {
        this->state = state;
        this->state->OnEnable();
    }

    State* getState() {
        return state;
    }

    void saveState(State* state) {
        savedState = state;
        savedState->OnDisable();
    }

    State* getSavedState() {
        return savedState;
    }

    void clearSavedState() {
        savedState = nullptr;
    }

    void saveExitState(State* state) {
        exitState = state;
    }

    State* getExitState() {
        return exitState;
    }

    void clearExitState() {
        exitState = nullptr;
    }
};
