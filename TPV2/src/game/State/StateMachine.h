#pragma once
class State;

class StateMachine {
protected:
    State* state = nullptr;
    State* savedState = nullptr;
public:
    void setState(State* state) {
        this->state = state;
    }

    State* getState() {
        return state;
    }

    void saveState(State* state) {
        savedState = state;
    }

    State* getSavedState() {
        return savedState;
    }

    void clearSavedState() {
        savedState = nullptr;
    }
};
