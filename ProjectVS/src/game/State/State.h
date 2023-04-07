#pragma once

#include <string>
#include <vector>

class FightManager;

class SDLUtils;
class Texture;
class InputHandler;
class PlayerConfigs;

using ushort = unsigned short;
using uchar = unsigned char;

class State {
protected:
    PlayerConfigs* playerPrefs;
    FightManager* fmngr;
    InputHandler* ih;
    SDLUtils* sdl;
    void showText(std::string s, ushort size, ushort x, ushort y, unsigned int hexColor);
    //double ts(double i);//TO SCREEN
    Texture* tex;
    std::vector<short> globalplayerInput;
public:
    State(FightManager* fmngr);

    virtual ~State() {    
    };
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void next() = 0;
    virtual void OnEnable() = 0;
    virtual void OnDisable() = 0;
    virtual void Reset();
    virtual void jump(State* state);
    virtual void jumpWithoutDelete(State* state);
    bool doQuit();
    virtual std::string getStateName() const = 0;
};