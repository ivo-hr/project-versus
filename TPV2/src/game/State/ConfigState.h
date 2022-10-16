#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"
#include "../Utils/PlayerSelectRect.h"

class ConfigState : public State {
    Texture* background , *charselbg;
    ushort numOfplayer;
    Button *gatoespia, *maketo ,*togo , *nasnas , *yuno , *aleatorio , *plusB, *minusB ,*play;
    Button* teammode, * normalmode , *team1, *team2 ,*config;
    bool TeamModebool = false;
    bool charsel = false;
    bool ready = false;
    bool searchGamepad = false;
    bool keyRelease = true;

    bool mapkeyRelease = true;

    bool selectMap = true;
    int map = -1;
    int lastPointerClick = -3;
    ushort nMandos;

    vector<short> playerInput;
    vector<short> charactersSelect;
    vector<short> charactersTeam;
    vector<bool> usedPad;
    vector<bool> usedKeyboard;
    vector<bool> selected;
    vector<PlayerPointer*> playerPointers;
    vector<PlayerSelectRect*> playerTexture;
    vector<Texture*> charactTexture;
    vector<vector<Button*>> p;
    vector<Button*> maps;
    vector<string> charName;
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
    void initcharact();
    int timerMapkeyRelease = 0;
public:

    ConfigState(FightManager* game , short fInput) ;
    virtual  ~ConfigState();

    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override {};
    void OnDisable() override {};

    string getStateName() const override {
        return "Config state";
    };
private:
    void setPointer();
};