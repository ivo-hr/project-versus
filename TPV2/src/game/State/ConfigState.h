#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"
#include "../Utils/PlayerSelectRect.h"

class ConfigState : public State {
    Texture* background, *exp;
    int numOfplayer;
    Button *zero, *gatoespia, *maketo ,*togo , *nasnas ,*aleatorio;

    int sel = 0;
    int key = 0;
    int nesn = 0;
    int xboxn = 0;
    bool charsel = false;
    bool searchGamepad = false;
 
    int k = -1;
    vector<int> playerInput;
    vector<int> charactersSelect;
    vector<bool> usedPad;
    vector<bool> usedKeyboard;
    vector<PlayerPointer*> playerPointers;
    vector<PlayerSelectRect*> playerTexture;
    vector<Texture*> charactTexture;
    
public:

    ConfigState(FightManager* game , int fInput) ;
    ~ConfigState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Config state";
    };
};