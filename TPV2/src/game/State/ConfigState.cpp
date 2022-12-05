#include "ConfigState.h"
#include "PlayingState.h"
#include "ExitState.h"
#include "../PlayingState/FightManager.h"
#include "../../utils/CheckML.h"
#include "../Utils/PlayerConfigs.h"

ConfigState::ConfigState(FightManager* game , short fInput) : State(game), numOfplayer(2) {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    initcharact();
    plusB = new Button(&sdl->images().at("pB"),  (int)(w * 14 / 15), (int)(h - w * 2.4f / 15), (int)(w / 15));
    minusB = new Button(&sdl->images().at("mB"), (int)(w * 14 / 15), (int)(h - w * 1.2f / 15), (int)(w / 15));
    play = new PlayButton(&sdl->images().at("play"), 0, 0, w, h);
    normalmode = new Button(&sdl->images().at("MNormalC"), &sdl->images().at("MNormalB"), (w / 2) + (h / 168), h / 168, h / 7, h / 14);
    normalmode->active(true);
    teammode = new Button(&sdl->images().at("MTeamC"), &sdl->images().at("MTeamB"), (w / 2) + h / 84 + h / 7, h / 168, h / 7, h / 14);
    config = new Button(&sdl->images().at("ConfigBut"), w * 20 / 21, 0 , w / 21, w / 21);
 
    configTeamChoose();
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P1")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P2")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P3")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P4")));
    initMapBut();
    usedKeyboard.resize(2);
    playerInput.resize(1);
    playerInput[0] = (char)fInput;
    playerTexture[0]->setgotInput(true);
    charactersSelect.resize(2);
    nMandos = SDL_NumJoysticks();
    usedPad.resize(4);
    selected.resize(4);
    if (fInput >= 0) { usedPad[fInput] = true; playerTexture[0]->setFront(&sdl->images().at("Mando")); }
    else if (fInput == -1) { usedKeyboard[0] = true; playerTexture[0]->setFront(&sdl->images().at("k1"));
    }
    else if (fInput == -2) { usedKeyboard[1] = true; playerTexture[0]->setFront(&sdl->images().at("k2"));
    }

    int dist = (w * 12 / 13) / numOfplayer;
    int offset = dist - w / 13;
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P1P"), 0u * dist + offset, 676, w, h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P2P"), 1u, 676, w, h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P3P"), 2u, 676, w, h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P4P"), 3u, 676, w, h));
    playerPointers[0]->setActive(true);
    sdl->musics().at("sawtines").play();

    SDL_ShowCursor(1);
}

ConfigState::~ConfigState()
{
    delete gatoespia;
    delete maketo;
    delete togo;
    delete nasnas;
    delete yuno;
    delete melvin;
    delete aleatorio;
    delete plusB;
    delete minusB;
    delete play;
    delete teammode;
    delete normalmode;
    delete config;
    for (auto e : playerPointers)delete e;
    for (auto e : playerTexture)delete e;
    for (auto e : charactTexture)delete e;
    for (auto e : p)
    {
        for (auto a : e)delete a;
    }
    for (auto e : maps)delete e;
}

void ConfigState::update() {

    if (!selectMap)
    {
        searchInput();
        movePointers();
        checkButtonPointerClick();
        checkButtonMouseClick();
        setTeams();
        checkPlayerReady();
      
    }
    else {
        movePointers();
        mapcheckButtonPointerClick();
        mapcheckButtonMouseClick();
        if (map >= 0) selectMap = false;
    }
    if (ready) {
        if (play->mouseClick()) {
            fmngr->getState()->next();
            return;
        }
        for (auto i = 0; i < playerInput.size(); i++) {
            bool enter = false;
            switch (playerInput[i])
            {
            case -1:
                if (ih.isKeyDown(SDLK_LCTRL))enter = true;
                break;
            case -2:
                if (ih.isKeyDown(SDLK_RCTRL))enter = true;
                break;
            default:
                if (ih.xboxGetButtonState(playerInput[i], SDL_CONTROLLER_BUTTON_B))enter = true;
                break;
            }
            if (play->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease) {
                fmngr->getState()->next();
                return;
            }
        }
    }
 
    if (config->mouseClick()) {
        if (fmngr->getSavedState() == nullptr) {
            //pause
            fmngr->saveState(fmngr->getState());
            fmngr->setState(new ConfigurationState(fmngr,playerInput[0]));
            return;
        }
        else
        {
            State* tmp = fmngr->getState();
            State* saved = fmngr->getSavedState();
            fmngr->setState(saved);
            fmngr->saveState(tmp);
            return;
        }
    }
    if (ih.isKeyDown(SDLK_ESCAPE) && ih.keyDownEvent()) {
        if (fmngr->getExitState() == nullptr) {
            //pause
            fmngr->saveState(fmngr->getState());
            fmngr->setState(new ExitState(fmngr));
            return;
        }
    }
}

void ConfigState::draw() {

    if (selectMap) {
        mapMenuRender();
    }
    else {
        playerMenuRender();
    }

    sdl->presentRenderer();
}

void ConfigState::next() {

    if (!TeamModebool) {
        fmngr->setState(new PlayingState(fmngr, playerInput, charactersSelect,map + 1)); //3 para el de noche
    }
    else
    {
        fmngr->setState(new PlayingState(fmngr, playerInput, charactersSelect,charactersTeam,map+1));
    }

    delete this;
}

void ConfigState::setPointer()
{
    int w = fmngr->GetActualWidth();
    int dist = (w * 12 / 13) / numOfplayer;
    int offset = dist - w / 13;
    playerPointers[playerInput.size() - 1]->setActive(true);
    playerPointers[playerInput.size() - 1]->setPosition(((int)playerInput.size() - 1) * dist + offset, (int)(fmngr->GetActualHeight() / 2.f));
}
void ConfigState::configTeamChoose()
{

    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();

    charactersTeam.resize(4);
    for (int i = 0; i < 4; i++)
    {
        vector<Button*> p1;
        p1.push_back(new Button(&sdl->images().at("T1C"), &sdl->images().at("T1B"), 0, 0, w / 30, w / 30));
        p1.push_back(new Button(&sdl->images().at("T2C"), &sdl->images().at("T2B"), 0, 0, w / 30, w / 30));
        p.push_back(p1);
    }
    for (int i = 0; i < 4; i++) {
        if (i < 2) {
            p[i][0]->active(true);
            charactersTeam[i] = 0;
        }
        else
        {
            p[i][1]->active(true);
            charactersTeam[i] = 1;
        }
      
    }
}

void ConfigState::searchInput()
{
    //Si hay algun input pendiente , buscarlo
    if (playerInput.size() < numOfplayer) {
        if (SDL_NumJoysticks() == nMandos)
        {
            for (auto i = 0; i < SDL_NumJoysticks(); i++) {
                if (ih.xboxGetAxesState(i, 1) == -1 && !usedPad[i]) {
                    usedPad[i] = true;
                    playerInput.push_back(i);
                    setPointer();
                    playerTexture[playerInput.size() - 1]->setgotInput(true);
                    playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("Mando"));
                    charactersSelect.resize(playerInput.size());
                    return;
                }
            }
        }
        else if (SDL_NumJoysticks() > nMandos) { //se ha enchufado nuevo mando
            vector<char> aux = playerInput;
            for (auto i = 0; i < SDL_NumJoysticks(); i++) {
                if (ih.xboxGetAxesState(i, 1) == -1) {
                    bool nes = false;
                    for (ushort j = 0u; j < aux.size(); j++)
                    {
                        if (i == aux[j]) {
                            nes = true;
                            j = (ushort)aux.size();
                        }
                    }
                    if (nes) {
                        for (ushort x = 0u; x < aux.size(); x++)
                        {
                            if (aux[x]>=0)
                            {
                                aux[x] = aux[x] + 1;
                                usedPad[aux[x]] = true;
                            }           
                        }
                    }
                    usedPad[i] = true;
                    aux.push_back(i);
                    playerInput = aux;
                    setPointer();
                    playerTexture[playerInput.size() - 1]->setgotInput(true);
                    playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("Mando"));
                    charactersSelect.resize(playerInput.size());
                    nMandos++;
                    return;
                }
            }
        }
        if (ih.isKeyDown(playerPrefs.Keyboard2Up()) && !usedKeyboard[1]) {
            usedKeyboard[1] = true;
            playerInput.push_back(-2);
            setPointer();
            playerTexture[playerInput.size() - 1]->setgotInput(true);
            playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("k2"));
        }
        if (ih.isKeyDown(playerPrefs.Keyboard1Up()) && !usedKeyboard[0]) {
            usedKeyboard[0] = true;
            playerInput.push_back(-1);
            setPointer();
            playerTexture[playerInput.size() - 1]->setgotInput(true);
            playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("k1"));
        }
        charactersSelect.resize(playerInput.size());
    }
}

void ConfigState::movePointers()
{
    //Movimiento de los punteros
    for (auto i = 0; i < playerInput.size(); i++) {
        switch (playerInput[i])
        {
        case -1:
            if (ih.isKeyDown(playerPrefs.Keyboard1Up()))playerPointers[i]->move(0);
            if (ih.isKeyDown(playerPrefs.Keyboard1Down()))playerPointers[i]->move(1);
            if (ih.isKeyDown(playerPrefs.Keyboard1Left()))playerPointers[i]->move(2);
            if (ih.isKeyDown(playerPrefs.Keyboard1Right()))playerPointers[i]->move(3);
            break;
        case -2:
            if (ih.isKeyDown(playerPrefs.Keyboard2Up()))playerPointers[i]->move(0);
            if (ih.isKeyDown(playerPrefs.Keyboard2Down()))playerPointers[i]->move(1);
            if (ih.isKeyDown(playerPrefs.Keyboard2Left()))playerPointers[i]->move(2);
            if (ih.isKeyDown(playerPrefs.Keyboard2Right()))playerPointers[i]->move(3);
            break;
        default:
            if (ih.xboxGetAxesState(playerInput[i], 1) == -1 || ih.xboxGetDpadState(playerInput[i], 0))
                playerPointers[i]->move(0);
            if (ih.xboxGetAxesState(playerInput[i], 1) == 1 || ih.xboxGetDpadState(playerInput[i], 2))
                playerPointers[i]->move(1);
            if (ih.xboxGetAxesState(playerInput[i], 0) == -1 || ih.xboxGetDpadState(playerInput[i], 3))
                playerPointers[i]->move(2);
            if (ih.xboxGetAxesState(playerInput[i], 0) == 1 || ih.xboxGetDpadState(playerInput[i], 1))
                playerPointers[i]->move(3);
            break;
        }
    }
}

void ConfigState::checkButtonPointerClick()
{
    

    //Comprobacion de punteros con los botones
    for (auto i = 0; i < playerInput.size(); i++) {
        bool enter = false;
        //Pulsacion de A
        switch (playerInput[i])
        {
        case -1:
            if (ih.isKeyDown(playerPrefs.Keyboard1Basic()))enter = true;
            break;
        case -2:
            if (ih.isKeyDown(playerPrefs.Keyboard2Basic()))enter = true;
            break;
        default:
            if (ih.xboxGetButtonState(playerInput[i], playerPrefs.ControllerBasic()))enter = true;
            break;
        }
        //Comprobacion con cada boton
        if (nasnas->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("nasNasSelect"));
            charactersSelect[i] = 0;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;

            sdl->soundEffects().at("uiMov").play();
        }
        else if (gatoespia->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("blinkMasterSelect"));
            charactersSelect[i] = 1;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;

            sdl->soundEffects().at("uiMov").play();
        }
        else if (togo->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("dinoSoulsSelect"));
            charactersSelect[i] = 2;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;

            sdl->soundEffects().at("uiMov").play();
        }
        else if (maketo->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("maktSelect"));
            charactersSelect[i] = 3;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;

            sdl->soundEffects().at("uiMov").play();
        }
        else if (yuno->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("yunoSelect"));
            charactersSelect[i] = 4;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;

            sdl->soundEffects().at("uiMov").play();
        }
        else if (melvin->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("melvinSelect"));
            charactersSelect[i] = 5;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;

            sdl->soundEffects().at("uiMov").play();
        }
        else if (aleatorio->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("aleatorioSelect"));
            charactersSelect[i] = -1;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;

            sdl->soundEffects().at("uiMov").play();
        }
        else if (minusB->pointerClick(playerPointers[i]->getRect()) && enter && numOfplayer > 2 && keyRelease) {
            numOfplayer--;

            sdl->soundEffects().at("uiMov").play();

            if (playerInput.size() > numOfplayer) {
                switch (playerInput[playerInput.size() - 1])
                {
                case -1:
                    usedKeyboard[0] = false;
                    break;
                case -2:
                    usedKeyboard[1] = false;
                    break;
                default:
                    usedPad[playerInput[playerInput.size() - 1]] = false;
                    break;
                }
                playerInput.resize(numOfplayer);
                playerPointers[playerInput.size()]->setActive(false);
                playerTexture[playerInput.size()]->setgotInput(false);
            }
            charactersSelect.resize(playerInput.size());
            keyRelease = false;
            lastPointerClick = playerInput[i];
        }
        else if (plusB->pointerClick(playerPointers[i]->getRect()) && enter && numOfplayer < 4 && keyRelease) {
            numOfplayer++;
            charactersSelect.resize(playerInput.size());
            keyRelease = false;
            lastPointerClick = playerInput[i];

            sdl->soundEffects().at("uiMov").play();
        }
        else if (normalmode->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease) {
            TeamModebool = false;
            normalmode->active(true);
            teammode->active(false);

            sdl->soundEffects().at("uiMov").play();
        }
        else if (teammode->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease) {
            TeamModebool = true;
            normalmode->active(false);
            teammode->active(true);

            sdl->soundEffects().at("uiMov").play();
        }
        else if (config->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease) {
            if (fmngr->getSavedState() == nullptr) {
                keyRelease = false;
                lastPointerClick = playerInput[i];
                //pause
                fmngr->saveState(fmngr->getState());
                fmngr->setState(new ConfigurationState(fmngr, playerInput[0]));

                return;
            }
            else
            {
                keyRelease = false;
                lastPointerClick = playerInput[i];
                State* tmp = fmngr->getState();
                State* saved = fmngr->getSavedState();
                fmngr->setState(saved);
                fmngr->saveState(tmp);
                return;
            }
        }
        //Teams
        for (auto j = 0u; j < 2; j++) {
            if (p[i][j]->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease) {
                if (j == 0) {
                    p[i][1]->active(false);
                    p[i][0]->active(true);
                    charactersTeam[i] = 0;
                }
                else
                {
                    p[i][1]->active(true);
                    p[i][0]->active(false);
                    charactersTeam[i] = 1;
                }
            }
        }
        //Release del Key
        if (!keyRelease) {
            switch (lastPointerClick)
            {
            case -1:
                if (!ih.isKeyDown(playerPrefs.Keyboard1Basic()))keyRelease = true;
                break;
            case -2:
                if (!ih.isKeyDown(playerPrefs.Keyboard2Basic()))keyRelease = true;
                break;
            case -3:
                break;
            default:
                if (!ih.xboxGetButtonState(lastPointerClick, playerPrefs.ControllerBasic()))keyRelease = true;
                break;
            }
        }
    
    }
    //Seleccion del personaje
    for (auto i = 0; i < playerInput.size(); i++) {
        switch (playerInput[i])
        {
        case -1:
            if (ih.isKeyDown(playerPrefs.Keyboard1Special()))selected[i] = false;
            break;
        case -2:
            if (ih.isKeyDown(playerPrefs.Keyboard2Special()))selected[i] = false;
            break;
        default:
            if (ih.xboxGetButtonState(playerInput[i], playerPrefs.ControllerSpecial()))selected[i] = false;
            break;
        }
    }
}

void ConfigState::checkButtonMouseClick()
{
    
    if (minusB->mouseClick() && numOfplayer > 2) {
        numOfplayer--;

        sdl->soundEffects().at("uiMov").play();

        if (playerInput.size() > numOfplayer) {
            switch (playerInput[playerInput.size() - 1])
            {
            case -1:
                usedKeyboard[0] = false;
                break;
            case -2:
                usedKeyboard[1] = false;
                break;
            default:
                usedPad[playerInput[playerInput.size() - 1]] = false;
                break;
            }
            playerInput.resize(numOfplayer);
            playerPointers[playerInput.size()]->setActive(false);
            playerTexture[playerInput.size()]->setgotInput(false);
        }
        charactersSelect.resize(playerInput.size());
    }
    else if (plusB->mouseClick() && numOfplayer < 4) {
        numOfplayer++;
        charactersSelect.resize(playerInput.size());

        sdl->soundEffects().at("uiMov").play();
    }
    else if (normalmode->mouseClick()) {
        TeamModebool = false;
        normalmode->active(true);
        teammode->active(false);

        sdl->soundEffects().at("uiMov").play();

    }
    else if (teammode->mouseClick()) {
        TeamModebool = true;
        normalmode->active(false);
        teammode->active(true);

        sdl->soundEffects().at("uiMov").play();
    }
}

void ConfigState::setTeams()
{
    for (auto i = 0u; i < playerInput.size(); i++) {
        for (auto j = 0u; j < 2; j++) {
            if (p[i][j]->mouseClick()) {
                if (j == 0) {
                    p[i][1]->active(false);
                    p[i][0]->active(true);
                    charactersTeam[i] = 0;
                }
                else
                {
                    p[i][1]->active(true);
                    p[i][0]->active(false);
                    charactersTeam[i] = 1;
                }
            }
        }
    }
}

void ConfigState::checkPlayerReady()
{
    //Empezar la partida
    for (auto i = 0u; i < numOfplayer; i++) {
        if (!selected[i]) {
            ready = false;
            play->reset();
            return;
        } 
    }
    if (ready)return;
    ready = true;
    sdl->soundEffects().at("uiPlay").play(0,1);
}
void ConfigState::playerMenuRender()
{
    background = &sdl->images().at("selectbg");
    charselbg = &sdl->images().at("selectcharbg");
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    for (auto c = 0u; c < 10; c++) {
        int dist = (w * 9 / 10) / 5;
        int offset = dist - w / 10;
        int j = c / 5;

        charselbg->render({ (int)(c % 5 * dist + offset), (int)(((w * 3 / 24) * j) + w / 12), (int)w / 12, (int)w / 12 });
        showText(charName[c], w / 80, (int)(c % 5 * dist + offset) - w / 24, (int)((((w * 3 / 24) * j) + w * 1.02f / 6)), build_sdlcolor(0x33FFE900));
    }
    int dist = (w * 9 / 10) / numOfplayer;
    int offset = dist - (int)w / 5;
    for (auto i = 0u; i < numOfplayer; i++) {
        playerTexture[i]->render((int)(i * dist + offset), (int)h * 2 / 3 + h / 24, (int)w / 5, (int)h / 4);
        if (selected[i])
            showText("Selected", w / 70, (int)(i * dist + offset + w / 25), (int)h * 2 / 24 + h * 2 / 3, build_sdlcolor(0x00FF0000));
    }

    if (TeamModebool) {
        for (auto i = 0u; i < playerInput.size(); i++) {

            for (int j = 0; j < 2; j++)
            {
                p[i][j]->setX((int)(i * dist + offset + w / 17.5f + j * (w / 20)));
                p[i][j]->setY((int)(h * 18.f / 20.f));
                p[i][j]->render();
            }
        }
    }
    nasnas->render();
    gatoespia->render();
    togo->render();
    maketo->render();
    yuno->render();
    melvin->render();
    plusB->render();
    minusB->render();
    normalmode->render();
    teammode->render();
    aleatorio->render();
    config->render();
    if (ready) {
        play->render(); 
    }

        
    for (auto e : playerPointers)e->render();
}


void ConfigState::initMapBut()
{

    int imgW = fmngr->GetActualWidth() / 6, imgH = fmngr->GetActualWidth() / 8;

    int totalW = (fmngr->GetActualWidth() * 3 / 5) + imgW;
    int offsetX = (fmngr->GetActualWidth() - totalW) / 2;

    int totalH = (fmngr->GetActualHeight() * 2 / 4) + imgH;
    int offsetY = (fmngr->GetActualHeight() - totalH) / 2;

    int i = 0;
    maps.push_back(new Button(&sdl->images().at("fondo"), offsetX + (fmngr->GetActualWidth() * (i % 4)) / 5, offsetY + fmngr->GetActualHeight() * (i / 4), imgW, imgH));
    i++;
    maps.push_back(new Button(&sdl->images().at("mazmorra"), offsetX + (fmngr->GetActualWidth() * (i % 4)) / 5, offsetY + fmngr->GetActualHeight() * (i / 4), imgW, imgH));
    i++;
    maps.push_back(new Button(&sdl->images().at("night"), offsetX + (fmngr->GetActualWidth() * (i % 4)) / 5, offsetY + fmngr->GetActualHeight() * (i / 4), imgW, imgH));
}


void ConfigState::mapMenuRender()
{
    background = &sdl->images().at("mapbg");
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    int dist = (w * 12 / 13) / numOfplayer;
    int offset = dist - w / 7;
    for(auto e: maps)
    {
        e->render();
    }
    config->render();
    playerPointers[0]->render();
}

void ConfigState::mapcheckButtonPointerClick()
{
    bool enter = false;
    //Pulsacion de A
    switch (playerInput[0])
    {
    case -1:
        if (ih.isKeyDown(playerPrefs.Keyboard1Basic()))enter = true;
        break;
    case -2:
        if (ih.isKeyDown(playerPrefs.Keyboard2Basic()))enter = true;
        break;
    default:
        if (ih.xboxGetButtonState(playerInput[0], playerPrefs.ControllerBasic()))enter = true;
        break;
    }
    for (int i = 0u; i < maps.size(); i++) {    
        if (maps[i]->pointerClick(playerPointers[0]->getRect())&&enter && keyRelease) {
            map = i;
            keyRelease = false;
            lastPointerClick = playerInput[0];

            sdl->soundEffects().at("uiMov").play();
        }
    }
    if (config->pointerClick(playerPointers[0]->getRect()) && enter && keyRelease) {
        keyRelease = false;
        if (fmngr->getSavedState() == nullptr) {
            //pause
            fmngr->saveState(fmngr->getState());
            fmngr->setState(new ConfigurationState(fmngr, playerInput[0]));
            return;
        }
        else
        {
            State* tmp = fmngr->getState();
            State* saved = fmngr->getSavedState();
            fmngr->setState(saved);
            fmngr->saveState(tmp);
            return;
        }
    }
    if (!keyRelease) {
        switch (playerInput[0])
        {
        case -1:
            if (!ih.isKeyDown(playerPrefs.Keyboard1Basic()))keyRelease = true;
            break;
        case -2:
            if (!ih.isKeyDown(playerPrefs.Keyboard2Basic()))keyRelease = true;
            break;
        case -3:
            break;
        default:
            if (!ih.xboxGetButtonState(playerInput[0], playerPrefs.ControllerBasic()))keyRelease = true;
            break;
        }
    }
   
}

void ConfigState::mapcheckButtonMouseClick()
{
    for (int i = 0u; i < maps.size(); i++) {
        if (maps[i]->mouseClick()) {
            map = i;
            sdl->soundEffects().at("uiSelect").play();
        }
    }
}

void ConfigState::initcharact()
{
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    int dist = (int)(((float)w * 9.f / 10.f) / 5.f);
    int offset = (int)(dist - (float)w / 10.f + (float)w / 68.7f);

    int distY = (int)((float)w * 3.f / 24.f);
    int offsetY = (int)((float)w / 12.f + (float)w / 70.f);

    int buttonSize = (int)((float)w / 15.4f);
    //c % 4 * dist + offset), (int)((ts(80) * j) + ts(50));

   // { 
   // (int)(c % 5 * dist + offset), 
   // (int)(((w * 3 / 24) * j) + w / 12), 
   //     (int)w / 12, (int)w / 12 }
 
    nasnas = new Button(&sdl->images().at("nasNasSelect"), offset , offsetY, buttonSize);
    charName.push_back("    NasNas");
    gatoespia = new Button(&sdl->images().at("blinkMasterSelect"), dist + offset, offsetY, buttonSize);
    charName.push_back(" Blink Master");
    maketo = new Button(&sdl->images().at("maktSelect"), dist * 2 + offset, offsetY, buttonSize);
    charName.push_back("  Makt Fange");
    togo = new Button(&sdl->images().at("dinoSoulsSelect"), dist * 3 + offset, offsetY, buttonSize);
    charName.push_back("     Togo");
    yuno = new Button(&sdl->images().at("yunoSelect"), dist * 4 + offset, offsetY, buttonSize);
    charName.push_back("     Yuno");
    melvin = new Button(&sdl->images().at("melvinSelect"), offset, distY + offsetY, buttonSize);
    charName.push_back("    Melvin");

    charName.push_back(" Coming Soon");
    charName.push_back(" Coming Soon");
    charName.push_back(" Coming Soon");

    aleatorio = new Button(&sdl->images().at("aleatorioSelect"), dist * 4 + offset, distY + offsetY, buttonSize);
    charName.push_back("    Random");
}
