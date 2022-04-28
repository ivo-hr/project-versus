#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"

class GameOverState : public State {
    Texture* background;
    //FightManager* fmngr;
    vector<Texture*>winnersTextures_;
    Button* playAgain;
    PlayerPointer* pointer;
    vector<int>playersInput_;

public:
    GameOverState(FightManager* fmngr, vector<Texture*>winnersTextures, vector<int>playersInput);
    ~GameOverState();

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "GameOver state";
    };
};