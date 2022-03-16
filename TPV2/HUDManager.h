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

class HUDManager {
public:
	gameData prueba1;

	void RenderText(const std::string, SDL_Color fgColor) {
		
	}

	virtual void init();
protected:
	HUDManager(float x, float y);
	~HUDManager();
	
	// Font currentFont;

	gameData prueba2;
};