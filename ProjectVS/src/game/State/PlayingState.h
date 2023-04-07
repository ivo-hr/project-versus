#pragma once

#include "State.h"

class Character;

class PlayingState : public State {
    
    std::vector<char> playersInput_;
    std::vector<short> characters_;
    std::vector<short> teams_;
    ushort map_;

    Character* GetCharacter(char index, short character, ushort pNum);
 
public:

    PlayingState(FightManager* fmngr, const std::vector<char>& playersInput, const std::vector<short>& characters, ushort map) ;
    PlayingState(FightManager* fmngr, const std::vector<char>& playersInput, const std::vector<short>& characters, const std::vector<short>& teams , ushort map);
    virtual ~PlayingState();

    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override;
    void OnDisable() override {};

    void Reset() override;

    std::string getStateName() const override {
        return "Playing state";
    };
};