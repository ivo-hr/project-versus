#include "ConfigurationState.h"

#include "../PlayingState/FightManager.h"


ConfigurationState::ConfigurationState(FightManager* game ,int pI) : State(game) {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    backgr = &sdl->images().at("ConfigBack");
    music = &sdl->images().at("BgmT");
    sfx = &sdl->images().at("SfxT");
    instru = &sdl->images().at("instru");
    exit = new Button(&sdl->images().at("backToMenu"), 0, h - ts(20), ts(40), ts(20));
    back = new Button(&sdl->images().at("BackBut"), ts(15), ts(15), ts(15), ts(15));

    muscm = new Button(&sdl->images().at("minusB"), ts(120), ts(105), ts(10), ts(10));
    muscp = new Button(&sdl->images().at("plusB"), ts(180),ts(105), ts(10), ts(10));
  
    sfxm = new Button(&sdl->images().at("minusB"), ts(120), ts(155), ts(10), ts(10));
    sfxp = new Button(&sdl->images().at("plusB"), ts(180), ts(155), ts(10), ts(10));

    
    p1 = new PlayerPointer(&sdl->images().at("P1P"), w/2, h/2, ts(15), ts(15), w, h);
    p1->setActive(true);
    pInput = pI;
}



void ConfigurationState::update() {

    bool enter = false;
    switch (pInput)
    {
    case -1:
        if (ih.isKeyDown(SDLK_w))p1->move(0);
        if (ih.isKeyDown(SDLK_s))p1->move(1);
        if (ih.isKeyDown(SDLK_a))p1->move(2);
        if (ih.isKeyDown(SDLK_d))p1->move(3);
        if (ih.isKeyDown(SDLK_e))enter = true;
        break;
    case -2:
        if (ih.isKeyDown(SDLK_UP))p1->move(0);
        if (ih.isKeyDown(SDLK_DOWN))p1->move(1);
        if (ih.isKeyDown(SDLK_LEFT))p1->move(2);
        if (ih.isKeyDown(SDLK_RIGHT))p1->move(3);
        if (ih.isKeyDown(SDLK_l))enter = true;
        break;
    default:
        if (ih.xboxGetAxesState(pInput, 1) == -1 || ih.xboxGetDpadState(pInput, 0))p1->move(0);
        if (ih.xboxGetAxesState(pInput, 1) == 1 || ih.xboxGetDpadState(pInput, 2))p1->move(1);
        if (ih.xboxGetAxesState(pInput, 0) == -1 || ih.xboxGetDpadState(pInput, 3))p1->move(2);
        if (ih.xboxGetAxesState(pInput, 0) == 1 || ih.xboxGetDpadState(pInput, 1))p1->move(3);
        if (ih.xboxGetButtonState(pInput, SDL_CONTROLLER_BUTTON_B))enter = true;
        break;
    }
    if (muscm->mouseClick() || muscm->pointerClick(p1->getRect())&&enter && keyRelease ) {
        if (musicV > 0) {
            musicV--;
            Music::setMusicVolume((128 * musicV) / 10);
            keyRelease = false;
        }
    }
    else if (muscp->mouseClick()  || muscp->pointerClick(p1->getRect()) && enter && keyRelease) {
        if (musicV < 10) {
            musicV++;
            Music::setMusicVolume((128 * musicV) / 10);
            keyRelease = false;
        }

    }
    else if (sfxm->mouseClick() || sfxm->pointerClick(p1->getRect()) && enter && keyRelease) {
        if (sfxV > 0) {
            sfxV--;
            SoundEffect::setChannelVolume((128 * sfxV) / 10);

            keyRelease = false;

            sdl->soundEffects().at("uiSelect").play(1);

        }
    }
    else if (sfxp->mouseClick() || sfxp->pointerClick(p1->getRect()) && enter && keyRelease) {
        if (sfxV < 10) {
            sfxV++;
            SoundEffect::setChannelVolume((128 * sfxV) / 10);

            keyRelease = false;

            sdl->soundEffects().at("uiSelect").play(1);

        }
    }
    if (ih.isKeyDown(SDLK_ESCAPE) && ih.keyDownEvent() || back->mouseClick() || back->pointerClick(p1->getRect()) && enter && keyRelease) {
        keyRelease = false;
        std::cout << "unpause" << std::endl;
        State* tmp = fmngr->getState();
        State* saved = fmngr->getSavedState();
        fmngr->setState(saved);
        fmngr->saveState(tmp);
        //fmngr->clearSavedState();
        //delete tmp;
        return;
    }
    if (exit->mouseClick() || exit->pointerClick(p1->getRect()) && enter && keyRelease) {
        keyRelease = false;
        std::cout << "unpause" << std::endl;
        State* tmp = fmngr->getState();
        State* saved = fmngr->getSavedState();
        delete saved;
        fmngr->saveState(tmp);
        fmngr->getState()->jumpWithoutDelete(new MenuState(fmngr));
        return;
    }
    enter = false;

    switch (pInput)
    {
    case -1:
        if (!ih.isKeyDown(SDLK_e))keyRelease = true;
        break;
    case -2:
        if (!ih.isKeyDown(SDLK_l))keyRelease = true;
        break;
    case -3:
        break;
    default:
        if (!ih.xboxGetButtonState(pInput, SDL_CONTROLLER_BUTTON_B))keyRelease = true;
        break;
    }
}
void ConfigurationState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
  
    backgr->render({ 0,0,w,h });
    showText("BGM", ts(8), ts(80), ts(107), build_sdlcolor(0x33FFFC00));
    music->render({ (int)ts(50),(int)ts(100),(int)ts(20),(int)ts(20) });
    showText(to_string(musicV), ts(8), ts(145), ts(107), build_sdlcolor(0x33FFFC00));
   
    showText("SFX", ts(8), ts(80), ts(157), build_sdlcolor(0x33FFFC00));
    sfx->render({ (int)ts(50),(int)ts(150),(int)ts(20),(int)ts(20) });
    showText(to_string(sfxV), ts(8), ts(145), ts(157), build_sdlcolor(0x33FFFC00));
    instru->render({ (int)ts(250),(int)ts(50),(int)ts(200),(int)ts(250) });
   
    sfxm->render();
    sfxp->render();
    muscm->render();
    muscp->render();
    exit->render();
    back->render();
    
    p1->render();
    sdl->presentRenderer();
}

void ConfigurationState::next() {
    cout << "Next State " << endl;
    delete this;
}