#pragma once

#include <string>
#include <SDL_ttf.h>
#include <vector>
#include <string>

#include "src/game/PlayingState/FightManager.h"

// Datos del jugador
struct infoJugador {
	std::string nombreJug;
	int numVidas,
		danyoRecibido;
};

// Datos de partida
struct gameData {
	int tiempoRestante,
		numJugadores;
	std::vector<infoJugador> infoJugador;
};

struct toDraw {
	Texture tex;
	SDL_Rect dest;
};

class HUDManager
{
public:
	gameData datos;

	void RenderText(const std::string, SDL_Color fgColor) {
		
	}

	HUDManager(float x, float y);
	~HUDManager();

	void AddPlayer(std::vector<Entity*> ent);
};