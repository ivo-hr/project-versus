#pragma once

#include "State.h"

class Character;

class PlayingState : public State {
    
    vector<char> playersInput_;
    vector<short> characters_;
    vector<short> teams_;
    ushort map_;

    Character* GetCharacter(char index, short character, ushort pNum);
 
public:

    PlayingState(FightManager* fmngr, const vector<char>& playersInput, const vector<short>& characters, ushort map) ;
    PlayingState(FightManager* fmngr, const vector<char>& playersInput, const vector<short>& characters, const vector<short>& teams , ushort map);
    virtual ~PlayingState();

    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override { SDL_ShowCursor(0); };
    void OnDisable() override {};

    void Reset() override;

    string getStateName() const override {
        return "Playing state";
    };
};