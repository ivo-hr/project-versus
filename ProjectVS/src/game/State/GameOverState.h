#pragma once

#include "State.h"

class PlayerPointer;
class Button;
class Texture;

class GameOverState : public State {
    Texture* background;
    //FightManager* fmngr;
    std::vector<Texture*>winnersTextures_;
    Button* playAgain;
    std::vector<PlayerPointer*> pointers = std::vector<PlayerPointer*>(1);
    PlayerPointer*& pointer;
    std::vector<char> allInputs = std::vector<char>();
    short playersInput_;
    std::vector<std::vector<ushort>>gameStats_;

    void drawGameStats();

public:
    GameOverState(FightManager* fmngr, std::vector<Texture*>winnersTextures, std::vector<std::vector<ushort>>gameStats, short playersInput, std::vector<char>playersInputV);
    virtual ~GameOverState();

    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override {};
    void OnDisable() override {};
    void Reset() override {};

    std::string getStateName() const override {
        return "GameOver state";
    };
};