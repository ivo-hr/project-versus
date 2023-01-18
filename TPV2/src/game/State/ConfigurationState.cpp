#include "ConfigurationState.h"
#include "../../utils/CheckML.h"
#include "../PlayingState/FightManager.h"
#include "../Utils/PlayerConfigs.h"


ConfigurationState::ConfigurationState(FightManager* game , short pI) : State(game), p1(pointers[0])
{
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    backgr = &sdl->images().at("ConfigBack");
    music = &sdl->images().at("BgmT");
    sfx = &sdl->images().at("SfxT");
    instru = &sdl->images().at("instru");

    pointers[0] = new PlayerPointer(&sdl->images().at("P1P"), w / 2, h / 2, w, h, pI);

    exit = new Button(&sdl->images().at("backToMenu"), 0, h - w / 12, w / 16, w / 12, pointers);
    exit->SetOnClick([this]() {ExitState(); });
    exit->SetOnPointerClick([this](int s) {ExitState(); });
    back = new Button(&sdl->images().at("BackBut"), 0, 0, w / 12, w / 12, pointers);
    back->SetOnClick([this]() {GoBack(); });
    back->SetOnPointerClick([this](int s) {GoBack(); });

    int butW = w / 50;

    muscm = new Button(&sdl->images().at("minusB"), (int)(w * 29 / 25 - (w * 5 / 6 + w / 16)), (int)((h / 4) + w / 50 - butW / 2), butW, butW, pointers);
    muscm->SetOnClick([this]() {DecreaseMusic(); });
    muscm->SetOnPointerClick([this](int s) {DecreaseMusic(); });
    muscp = new Button(&sdl->images().at("plusB"), (int)(w * 33 / 25 - (w * 5 / 6 + w / 16)), (int)((h / 4) + w / 50 - butW / 2), butW, butW, pointers);
    muscp->SetOnClick([this]() {IncreaseMusic(); });
    muscp->SetOnPointerClick([this](int s) {IncreaseMusic(); });
  
    sfxm = new Button(&sdl->images().at("minusB"), (int)(w * 29 / 25 - (w * 5 / 6 + w / 16)), (int)((h * 1.3f / 4) + w / 50 - butW / 2), butW, butW, pointers);
    sfxm->SetOnClick([this]() {DecreaseSFX(); });
    sfxm->SetOnPointerClick([this](int s) {DecreaseSFX(); });
    sfxp = new Button(&sdl->images().at("plusB"), (int)(w * 33 / 25 - (w * 5 / 6 + w / 16)), (int)((h * 1.3f / 4) + w / 50 - butW / 2), butW, butW, pointers);
    sfxp->SetOnClick([this]() {IncreaseSFX(); });
    sfxp->SetOnPointerClick([this](int s) {IncreaseSFX(); });

    fullSCheck = new ToggleButton(&sdl->images().at("check"), (int)(w * 33 / 25 - (w * 5 / 6 + w / 16)), (int)(h * 1.2f / 2 - butW), butW * 2, butW * 2, pointers);
    fullSCheck->SetEnabled(SDL_GetWindowFlags(sdl->window()) & SDL_WINDOW_FULLSCREEN);
    fullSCheck->SetOnClick([this]() {ToggleFullScreen(); });
    fullSCheck->SetOnPointerClick([this](int s) {ToggleFullScreen(); });
    
    p1 = pointers[0];
    p1->setActive(true);
    pInput = pI;

    SDL_ShowCursor(1);
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
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

void ConfigurationState::update()
{
    fullSCheck->update();

    sfxm->update();
    sfxp->update();
    muscm->update();
    muscp->update();
    exit->update();
    back->update();

    pointers[0]->update();
}

void ConfigurationState::IncreaseSFX()
{
    if (sfxV < 10) {
        sfxV++;
        SoundEffect::setChannelVolume((int)(128 * sfxV) / 10);
        SoundEffect::setChannelVolume((int)(128 * sfxV) / 10, 1);

        keyRelease = false;

        sdl->soundEffects().at("uiSelect").play(1);
        toReDraw = true;

    }
}
void ConfigurationState::DecreaseSFX()
{
    if (sfxV > 0) {
        sfxV--;
        SoundEffect::setChannelVolume((int)(128 * sfxV) / 10);
        SoundEffect::setChannelVolume((int)(128 * sfxV) / 10, 1);

        keyRelease = false;

        sdl->soundEffects().at("uiSelect").play(1);
        toReDraw = true;

    }
}
void ConfigurationState::IncreaseMusic()
{
    if (musicV < 10) {
        musicV++;
        Music::setMusicVolume((int)(128 * musicV) / 10);
        keyRelease = false;
        toReDraw = true;
    }
}
void ConfigurationState::DecreaseMusic()
{
    if (musicV > 0) {
        musicV--;
        Music::setMusicVolume((int)(128 * musicV) / 10);
        keyRelease = false;
        toReDraw = true;
    }
}
void ConfigurationState::GoBack()
{
    keyRelease = false;
    toReDraw = true;
    State* tmp = fmngr->getState();
    State* saved = fmngr->getSavedState();
    fmngr->setState(saved);
    fmngr->saveState(tmp);
}
void ConfigurationState::ExitState()
{
    keyRelease = false;
    toReDraw = true;
    State* tmp = fmngr->getState();
    State* saved = fmngr->getSavedState();
    delete saved;
    fmngr->saveState(tmp);
    fmngr->setState(new MenuState(fmngr));
}
void ConfigurationState::ToggleFullScreen()
{

    SDL_MaximizeWindow(sdl->window());
    sdl->toggleFullScreen();

    auto flags = SDL_GetWindowFlags(sdl->window());
    if (!(flags & SDL_WINDOW_FULLSCREEN))
    {
        SDL_SetWindowResizable(sdl->window(), SDL_TRUE);
    }

    fullSCheck->SetEnabled(flags & SDL_WINDOW_FULLSCREEN);
    toReDraw = true;
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


    showText("FULLSCREEN", fontSiz, (int)(w - (w * 5 / 6 + w / 16)), (int)(h * 1.2f / 2 - fontSiz / 2), build_sdlcolor(0x33FFFC00));

    fullSCheck->render();
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
    delete this;
}