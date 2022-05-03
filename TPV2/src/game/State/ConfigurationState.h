#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"

class ConfigurationState : public State {
    Texture* backgr, * sfx, * music, * c1, * c2,*instru;
    Button* sfxp, * sfxm, * muscp, * muscm;
    Button* exit , *back;
    PlayerPointer *p1;

    int musicV = 10;
    int sfxV = 10;
    int pInput = -3;
    bool keyRelease=true;
public:

    ConfigurationState(FightManager* fmngr ,int pI);
    virtual  ~ConfigurationState();

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Pause state";
    };
};