#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"
#include "../Utils/PlayerSelectRect.h"

class ConfigState : public State {
    Texture* background;
    int numOfplayer;
    Button *zero, *gatoespia, *maketo ,*togo , *nasnas ,*aleatorio , *plusB, *minusB ,*play;
    Button* teammode, * normalmode , *team1, *team2 ,*config;
    bool TeamModebool = false;
    bool charsel = false;
    bool ready = false;
    bool searchGamepad = false;
    bool keyRelease = true;
    bool selectMap = true;
    int map = -1;
    int lastPointerClick = -3;

    vector<int> playerInput;
    vector<int> charactersSelect;
    vector<int> charactersTeam;
    vector<bool> usedPad;
    vector<bool> usedKeyboard;
    vector<bool> selected;
    vector<PlayerPointer*> playerPointers;
    vector<PlayerSelectRect*> playerTexture;
    vector<Texture*> charactTexture;
    vector<vector<Button*>> p;
    vector<Button*> maps;

    void configTeamChoose();
   
    void searchInput();
    void movePointers();
    void checkButtonPointerClick();
    void checkButtonMouseClick();
    void setTeams();
    void checkPlayerReady();
    void initMapBut();
    void playerMenuRender();
    void mapMenuRender();
    void mapcheckButtonPointerClick();
    void mapcheckButtonMouseClick();

public:

    ConfigState(FightManager* game , int fInput) ;
    ~ConfigState();

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Config state";
    };
private:
    void setPointer();
};