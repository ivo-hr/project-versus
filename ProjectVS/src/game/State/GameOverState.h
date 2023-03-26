#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"

class GameOverState : public State {
    Texture* background;
    //FightManager* fmngr;
    vector<Texture*>winnersTextures_;
    Button* playAgain;
    vector<PlayerPointer*> pointers = vector<PlayerPointer*>(1);
    PlayerPointer*& pointer;
    vector<char> allInputs = vector<char>();
    short playersInput_;
    vector<vector<ushort>>gameStats_;

    void drawGameStats();

public:
    GameOverState(FightManager* fmngr, vector<Texture*>winnersTextures, vector<vector<ushort>>gameStats, short playersInput, vector<char>playersInputV);
    virtual ~GameOverState();

    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override {};
    void OnDisable() override {};
    void Reset() override {};

    string getStateName() const override {
        return "GameOver state";
    };
};