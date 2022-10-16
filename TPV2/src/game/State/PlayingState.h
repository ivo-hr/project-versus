#pragma once

#include "State.h"

class PlayingState : public State {
    
    vector<short>playersInput_;
 
public:

    PlayingState(FightManager* fmngr , vector<short>player, vector<short>characters, ushort map) ;
    PlayingState(FightManager* fmngr, vector<short>player, vector<short>characters, vector<short>teams , ushort map);
    virtual  ~PlayingState();

    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override { SDL_ShowCursor(0); };
    void OnDisable() override {};

    string getStateName() const override {
        return "Playing state";
    };
};