#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"
#include "../Utils/PlayerSelectRect.h"

class ConfigState : public State {
    Texture* background , *charselbg;
    ushort numOfplayer;

    const Texture* stageTextures[3];

    vector<Button*> buttons = vector<Button*>(10);

    Button *play;
    ToggleButton* teammode, * normalmode;
    bool TeamModebool = false;
    bool charsel = false;
    bool ready = false;
    bool searchGamepad = false;
    bool keyRelease = true;

    bool mapkeyRelease = true;

    bool selectMap = true;
    int mapChosen = -1;
    int lastPointerClick = -3;
    ushort nMandos;

    vector<char> playerInput;
    vector<short> charactersSelect;
    vector<short> charactersTeam;
    vector<bool> usedPad;
    vector<bool> usedKeyboard;
    vector<bool> selected;
    vector<PlayerPointer*> playerPointers;
    vector<PlayerSelectRect*> playerTexture;
    vector<Texture*> charactTexture;
    vector<vector<ToggleButton*>> p;
    vector<Button*> maps;
    vector<string> charName;
    void configTeamChoose();
   
    void searchInput();
    void movePointers();
    void checkButtonPointerClick();
    void checkPlayerReady();
    void initMapBut();
    void playerMenuRender();
    void mapMenuRender();
    void initcharact();

    void SelectCharacter(int i, const string& name, char character);
    void RemovePlayer();
    void AddPlayer();
    void ChangeTeam(int pl, bool t1);
    void OpenConfig();
    int timerMapkeyRelease = 0;

    vector<char> AuxFunc(char inp);
public:

    ConfigState(FightManager* game , short fInput);
    ConfigState(FightManager* game , const vector<char>& inputs);
    ~ConfigState();

    void InitAllButtons(int w, int h);
    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override {};
    void OnDisable() override {};
    void Reset() override {};

    string getStateName() const override {
        return "Config state";
    };
private:
    void setPointer(short input);
};