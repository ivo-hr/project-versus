#include "ConfigurationState.h"
#include "../../utils/CheckML.h"
#include "../PlayingState/FightManager.h"


ConfigurationState::ConfigurationState(FightManager* game , short pI) : State(game) {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    backgr = &sdl->images().at("ConfigBack");
    music = &sdl->images().at("BgmT");
    sfx = &sdl->images().at("SfxT");
    instru = &sdl->images().at("instru");
    exit = new Button(&sdl->images().at("backToMenu"), 0, h - w / 12, w / 16, w / 12);
    back = new Button(&sdl->images().at("BackBut"), 0, 0, w / 12, w / 12);

    int butW = w / 50;

    muscm = new Button(&sdl->images().at("minusB"), (int)(w * 29 / 25 - (w * 5 / 6 + w / 16)), (int)((h / 4) + w / 50 - butW / 2), butW, butW);
    muscp = new Button(&sdl->images().at("plusB"), (int)(w * 33 / 25 - (w * 5 / 6 + w / 16)), (int)((h / 4) + w / 50 - butW / 2), butW, butW);
  
    sfxm = new Button(&sdl->images().at("minusB"), (int)(w * 29 / 25 - (w * 5 / 6 + w / 16)), (int)((h * 1.3f / 4) + w / 50 - butW / 2), butW, butW);
    sfxp = new Button(&sdl->images().at("plusB"), (int)(w * 33 / 25 - (w * 5 / 6 + w / 16)), (int)((h * 1.3f / 4) + w / 50 - butW / 2), butW, butW);

    fullSCheck = new Button(&sdl->images().at("check"), (int)(w * 33 / 25 - (w * 5 / 6 + w / 16)), (int)(h * 1.2f / 2 - butW), butW * 2, butW * 2);
    
    p1 = new PlayerPointer(&sdl->images().at("P1P"), w/2, h/2, w, h);
    p1->setActive(true);
    pInput = pI;
}

ConfigurationState::~ConfigurationState()
{
    delete sfxp;
    delete sfxm;
    delete muscp;
    delete muscm;
    delete exit;
    delete back;
    delete p1;
    delete fullSCheck;
}



void ConfigurationState::update() {

    bool enter = false;
    switch (pInput)
    {
    case -1:
        if (ih.isKeyDown(SDLK_w)) { p1->move(0); toReDraw = true; }
        if (ih.isKeyDown(SDLK_s)){ p1->move(1); toReDraw = true; }
        if (ih.isKeyDown(SDLK_a)) { p1->move(2); toReDraw = true; }
        if (ih.isKeyDown(SDLK_d)) { p1->move(3); toReDraw = true; }
        if (ih.isKeyDown(SDLK_LCTRL)) { enter = true; toReDraw = true; }
        break;
    case -2:
        if (ih.isKeyDown(SDLK_UP)) { p1->move(0); toReDraw = true; }
        if (ih.isKeyDown(SDLK_DOWN)) { p1->move(1); toReDraw = true; }
        if (ih.isKeyDown(SDLK_LEFT)) { p1->move(2); toReDraw = true; }
        if (ih.isKeyDown(SDLK_RIGHT)) { p1->move(3); toReDraw = true; }
        if (ih.isKeyDown(SDLK_RCTRL)) { enter = true; toReDraw = true; }
        break;
    default:
        if (ih.xboxGetAxesState(pInput, 1) == -1 || ih.xboxGetDpadState(pInput, 0)) { p1->move(0); toReDraw = true; }
        if (ih.xboxGetAxesState(pInput, 1) == 1 || ih.xboxGetDpadState(pInput, 2)) { p1->move(1); toReDraw = true; }
        if (ih.xboxGetAxesState(pInput, 0) == -1 || ih.xboxGetDpadState(pInput, 3)) { p1->move(2); toReDraw = true; }
        if (ih.xboxGetAxesState(pInput, 0) == 1 || ih.xboxGetDpadState(pInput, 1)) { p1->move(3); toReDraw = true; }
        if (ih.xboxGetButtonState(pInput, SDL_CONTROLLER_BUTTON_B)) { enter = true; toReDraw = true; }
        break;
    }
    if (muscm->mouseClick() || muscm->pointerClick(p1->getRect())&&enter && keyRelease ) {
        if (musicV > 0) {
            musicV--;
            Music::setMusicVolume((int)(128 * musicV) / 10);
            keyRelease = false;
            toReDraw = true;
        }
    }
    else if (muscp->mouseClick()  || muscp->pointerClick(p1->getRect()) && enter && keyRelease) {
        if (musicV < 10) {
            musicV++;
            Music::setMusicVolume((int)(128 * musicV) / 10);
            keyRelease = false;
            toReDraw = true;
        }

    }
    else if (sfxm->mouseClick() || sfxm->pointerClick(p1->getRect()) && enter && keyRelease) {
        if (sfxV > 0) {
            sfxV--;
            SoundEffect::setChannelVolume((int)(128 * sfxV) / 10);
            SoundEffect::setChannelVolume((int)(128 * sfxV) / 10,1);

            keyRelease = false;

            sdl->soundEffects().at("uiSelect").play(1);
            toReDraw = true;

        }
    }
    else if (sfxp->mouseClick() || sfxp->pointerClick(p1->getRect()) && enter && keyRelease) {
        if (sfxV < 10) {
            sfxV++;
            SoundEffect::setChannelVolume((int)(128 * sfxV) / 10);
            SoundEffect::setChannelVolume((int)(128 * sfxV) / 10,1);

            keyRelease = false;

            sdl->soundEffects().at("uiSelect").play(1);
            toReDraw = true;

        }
    }
    else if (fullSCheck->mouseClick() || fullSCheck->pointerClick(p1->getRect()) && enter && keyRelease)
    {
        SDL_MaximizeWindow(sdl->window());
        sdl->toggleFullScreen();

        auto flags = SDL_GetWindowFlags(sdl->window());
        if (!(flags & SDL_WINDOW_FULLSCREEN))
        {
            SDL_SetWindowResizable(sdl->window(), SDL_TRUE);
        }

        keyRelease = false;
        toReDraw = true;

    }
    if (ih.isKeyDown(SDLK_ESCAPE) && ih.keyDownEvent() || back->mouseClick() || back->pointerClick(p1->getRect()) && enter && keyRelease) {
        keyRelease = false;
        toReDraw = true;
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
        toReDraw = true;
        std::cout << "unpause" << std::endl;
        State* tmp = fmngr->getState();
        State* saved = fmngr->getSavedState();
        delete saved;
        fmngr->saveState(tmp);
        fmngr->setState(new MenuState(fmngr));
        return;
    }
    enter = false;

    switch (pInput)
    {
    case -1:
        if (!ih.isKeyDown(SDLK_LCTRL))keyRelease = true;
        break;
    case -2:
        if (!ih.isKeyDown(SDLK_RCTRL))keyRelease = true;
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

    SDL_Rect instruRect = { (int)((w / 16) + (w / 2)), (int)(h / 4), (int)(w / 3), (int)(h * 0.7f) };

    int fontSiz = h / 32;

    showText("BGM", fontSiz, (int)(w * 27 / 25 - (w * 5 / 6 + w / 16)), (int)((h / 4) + w / 50 - fontSiz / 2), build_sdlcolor(0x33FFFC00));

    music->render({ (int)(w - (w * 5 / 6 + w / 16)), (int)h / 4, (int)w / 25, (int)w / 25 });

    showText(to_string(musicV), fontSiz, (int)(w * 31 / 25 - (w * 5 / 6 + w / 16)), (int)((h / 4) + w / 50 - fontSiz / 2), build_sdlcolor(0x33FFFC00));


    showText("SFX", fontSiz, (int)(w * 27 / 25 - (w * 5 / 6 + w / 16)), (int)((h * 1.3f / 4) + w / 50 - fontSiz / 2), build_sdlcolor(0x33FFFC00));

    sfx->render({ (int)(w - (w * 5 / 6 + w / 16)), (int)(h * 1.3f / 4), (int)w / 25, (int)w / 25 });

    showText(to_string(sfxV), fontSiz, (int)(w * 31 / 25 - (w * 5 / 6 + w / 16)), (int)((h * 1.3f / 4) + w / 50 - fontSiz / 2), build_sdlcolor(0x33FFFC00));


    showText("FULLSCREEN", fontSiz, (int)(w - (w * 5 / 6 + w / 16)), (int)h * 1.2f / 2 - fontSiz / 2, build_sdlcolor(0x33FFFC00));

    auto flags = SDL_GetWindowFlags(sdl->window());
    if (flags & SDL_WINDOW_FULLSCREEN)
    {
        fullSCheck->render({ 0, 32, 32, 32 });
    }
    else
    {
        fullSCheck->render({ 0, 0, 32, 32 });
    }


    instru->render(instruRect);

    sfxm->render();
    sfxp->render();
    muscm->render();
    muscp->render();
    exit->render();
    back->render();

    p1->render();
    sdl->presentRenderer();

    toReDraw = false;
}

void ConfigurationState::next() {
    cout << "Next State " << endl;
    delete this;
}