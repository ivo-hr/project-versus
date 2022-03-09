#pragma once

#include <string>
#include <SDL_ttf.h>
#include <vector>

#include "src/game/PlayingState/FightManager.h"

// Datos del jugador
struct infoJugador {
	int numVidas,
		danyoRecibido;

};

// Datos de partida
struct gameData {
	int tiempoRestante,
		numJugadores;
	std::vector<infoJugador> datosJug;
};

struct toDraw {
	Texture tex;
	SDL_Rect dest;
};

class HUDManager {
public:
	gameData prueba1;

	void renderText(const std::string, SDL_Color fgColor) {

	}

	virtual void init();
protected:
	HUDManager();
	~HUDManager();
	
	Font currentFont;

	gameData prueba2;
};