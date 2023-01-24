#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"

class ConfigurationState : public State {
    Texture* backgr, * sfx, * music, * c1, * c2,*instru;
    Button* sfxp, * sfxm, * muscp, * muscm;
    ToggleButton *fullSCheck;
    Button* exit , *back;
    vector<PlayerPointer*> pointers = vector<PlayerPointer*>(1);
    PlayerPointer*& p1;

    int musicV = 10;
    int sfxV = 10;
    int pInput = -3;
    bool keyRelease = true;

    bool toReDraw = true;
public:

    ConfigurationState(FightManager* fmngr , short pI);
    virtual  ~ConfigurationState();

    void update() override;
    void IncreaseSFX();
    void DecreaseSFX();
    void IncreaseMusic();
    void DecreaseMusic();
    void GoBack();
    void ExitState();
    void ToggleFullScreen();
    void draw() override;
    void next() override;
    void OnEnable() override { SDL_ShowCursor(1); };
    void OnDisable() override {};

    string getStateName() const override {
        return "Pause state";
    };
};