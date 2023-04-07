#pragma once

#include "State.h"

class Texture;
class Button;

class MenuState : public State {
    Texture* background;
    char Finput = -3;
    unsigned int textTimer = 0;
    bool drawText = false;
    Button* exit ,*config;
public:

    MenuState(FightManager* game);
    virtual ~MenuState();

    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override {};
    void OnDisable() override {};
    void Reset() override {};

    std::string getStateName() const override {
        return "Menu state";
    };
};
