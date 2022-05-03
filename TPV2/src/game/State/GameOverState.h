#pragma once

#include "State.h"

#include "../Utils/PlayerPointer.h"

class GameOverState : public State {
    Texture* background;
    //FightManager* fmngr;
    vector<Texture*>winnersTextures_;
    Button* playAgain;
    PlayerPointer* pointer;
    int playersInput_;
    vector<vector<int>>gameStats_;

    void drawGameStats();

public:
    GameOverState(FightManager* fmngr, vector<Texture*>winnersTextures, vector<vector<int>>gameStats, int playersInput, vector<int>playersInputV);
    virtual ~GameOverState();

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "GameOver state";
    };
};