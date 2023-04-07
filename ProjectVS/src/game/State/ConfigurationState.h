#pragma once

#include "State.h"
//#include "../Utils/PlayerPointer.h"

class PlayerPointer;
class Button;
class ToggleButton;

class ConfigurationState : public State {
    Texture* backgr, * sfx, * music, *instru;
    Button* sfxp, * sfxm, * muscp, * muscm;
    ToggleButton *fullSCheck;
    Button* exit , *back;
    std::vector<PlayerPointer*> pointers = std::vector<PlayerPointer*>(1);
    PlayerPointer*& p1;

    int musicV = 10;
    int sfxV = 10;
    int pInput = -3;
    bool keyRelease = true;

    bool toReDraw = true;
    bool exited = false;
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
    void OnEnable() override;
    void OnDisable() override {};
    void Reset() override {};

    std::string getStateName() const override {
        return "Pause state";
    };
};