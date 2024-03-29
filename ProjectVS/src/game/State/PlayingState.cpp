﻿#include "PlayingState.h"
#include "GameOverState.h"
#include "../PlayingState/FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../../../CharInclude.h"
#include "../../utils/CheckML.h"
#include "../Utils/PlayerConfigs.h"



PlayingState::PlayingState(FightManager* game, const vector<char>& playersInput, const vector<short>& characters , ushort map) : State(game) {

	playersInput_ = playersInput;
	characters_ = characters;
	teams_ = vector<short>();
	teams_.clear();
	map_ = map;

	Reset();
}

PlayingState::PlayingState(FightManager* game, const vector<char>& playersInput, const vector<short>& characters, const vector<short>& teams, ushort map) : State(game)
{
	playersInput_ = playersInput;
	characters_ = characters;
	teams_ = teams;
	map_ = map;

	Reset();
}

PlayingState::~PlayingState()
{
	fmngr->onNewGame();
}

Character* PlayingState::GetCharacter(char input, short character, ushort pNum)
{
	auto i = pNum;
	switch (character)
	{
	case 0: //nasnas
		return new NasNas(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input,i);
		break;
	case 1: //Gato espia
		return new GatoEspia(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input,i);
		break;
	case 2: //Maketo
		return new Makt(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
		break;
	case 3: //Togo
		return new Togo(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input,i);
		break;
	case 4: //Yuno
		return new Yuno(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
		break;
	case 5: //Melvin
		return new Melvin(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
		break;
	case -1://Aleatorio
		int charac = sdl->rand().nextInt(0, 13);

		switch (input)
		{
		case -1:
			if (ih->isKeyDown(playerPrefs->Keyboard1Down()))charac = 12;
			break;
		case -2:
			if (ih->isKeyDown(playerPrefs->Keyboard2Down()))charac = 12;
			break;
		default:
			if (ih->xboxGetAxesState(input, 1) == 1 || ih->xboxGetDpadState(input, 2))
				charac = 12;
			break;
		}

#ifdef _DEBUG
		charac = 12;		//Si estamos en debug siempre sale Ciro
#endif // _DEBUG

		switch (charac)
		{
		case 0: //nasnas
		case 1:
			return new NasNas(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
			break;
		case 2://Gato espia
		case 3:
			return new GatoEspia(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
			break;
		case 4://Togo
		case 5:
			return new Togo(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
			break;
		case 6: //Maketo
		case 7:
			return new Makt(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
			break;
		case 8: //Yuno
		case 9:
			return new Yuno(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
			break;
		case 10: //Melvin
		case 11:
			return new Melvin(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
			break;
		case 12: //Zero
			return new CharacterZero(fmngr, b2Vec2((float)(20 + i * 10), 0.f), input, i);
			break;
		}
		break;
	}
	return nullptr;
}


void PlayingState::update() {
	fmngr->Update();
}

void PlayingState::draw() {
	fmngr->DrawFight();
}

void PlayingState::next() {
	vector<Texture*>winnersTextures = fmngr->getWinnersTextures();
	vector<vector<ushort>>gameStats = fmngr->getGameStats();
	fmngr->setState(new GameOverState(fmngr, winnersTextures, gameStats, fmngr->getWinnerInput(), playersInput_));
	delete this;
}

void PlayingState::OnEnable() {
	SDL_ShowCursor(0);
}

void PlayingState::Reset()
{
	if (teams_.empty())
	{
		std::vector<Character*> entities;

		string s = "resources/config/Stages/stage" + to_string(map_) + ".json";
		fmngr->LoadStage(s);

		for (auto i = 0u; i < playersInput_.size(); i++)
		{
			entities.push_back(GetCharacter(playersInput_[i], characters_[i], i));
		}

		SDL_ShowCursor(0);

		fmngr->StartFight(entities);
	}
	else
	{
		std::vector<Character*> team1;
		std::vector<Character*> team2;
		string s = "resources/config/Stages/stage" + to_string(map_) + ".json";
		fmngr->LoadStage(s);

		for (auto i = 0u; i < playersInput_.size(); i++)
		{
			if (teams_[i] == 0)
				team1.push_back(GetCharacter(playersInput_[i], characters_[i], i));
			else
				team2.push_back(GetCharacter(playersInput_[i], characters_[i], i));
		}

		SDL_ShowCursor(0);

		fmngr->StartFight(team1, team2);
	}
}
