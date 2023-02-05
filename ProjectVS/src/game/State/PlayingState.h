#pragma once

#include "State.h"

class Character;

class PlayingState : public State {
    
    vector<char>playersInput_;
 
public:

    PlayingState(FightManager* fmngr , vector<char>player, vector<short>characters, ushort map) ;
    PlayingState(FightManager* fmngr, vector<char>player, vector<short>characters, vector<short>teams , ushort map);
    virtual  ~PlayingState();

    Character* GetCharacter(char index, char character, ushort pNum);

    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override { SDL_ShowCursor(0); };
    void OnDisable() override {};

    string getStateName() const override {
        return "Playing state";
    };
};