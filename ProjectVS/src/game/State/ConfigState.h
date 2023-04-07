#pragma once

#include "State.h"
//#include "../Utils/PlayerPointer.h"
//#include "../Utils/PlayerSelectRect.h"

class ToggleButton;
class Button;
class PlayerPointer;
class PlayerSelectRect;
class Texture;

class ConfigState : public State {
    Texture* background , *charselbg;
    ushort numOfplayer;

    const Texture* stageTextures[3];

    std::vector<Button*> buttons = std::vector<Button*>(10);

    Button* play;
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

    std::vector<char> playerInput;
    std::vector<short> charactersSelect;
    std::vector<short> charactersTeam;
    std::vector<bool> usedPad;
    std::vector<bool> usedKeyboard;
    std::vector<bool> selected;
    std::vector<PlayerPointer*> playerPointers;
    std::vector<PlayerSelectRect*> playerTexture;
    std::vector<Texture*> charactTexture;
    std::vector<std::vector<ToggleButton*>> p;
    std::vector<Button*> maps;
    std::vector<std::string> charName;
    void configTeamChoose();
   
    void searchInput();
    void movePointers();
    void checkButtonPointerClick();
    void checkPlayerReady();
    void initMapBut();
    void playerMenuRender();
    void mapMenuRender();
    void initcharact();

    void SelectCharacter(int i, const std::string& name, char character);
    void RemovePlayer();
    void AddPlayer();
    void ChangeTeam(int pl, bool t1);
    void OpenConfig();
    int timerMapkeyRelease = 0;

    std::vector<char> AuxFunc(char inp);
public:

    ConfigState(FightManager* game , char fInput);
    ConfigState(FightManager* game , const std::vector<char>& inputs);
    ~ConfigState();

    void InitAllButtons(int w, int h);
    void update() override;
    void draw() override;
    void next() override;
    void OnEnable() override {};
    void OnDisable() override {};
    void Reset() override {};

    std::string getStateName() const override {
        return "Config state";
    };
private:
    void setPointer(short input);
};