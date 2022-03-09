#pragma once

#include <string>
#include <SDL_ttf.h>
#include <vector>

#include "src/game/PlayingState/FightManager.h"

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

// Datos de partida
struct gameData {
	int tiempoRestante,
		numJugadores;
	std::vector<infoJugador> datosJug;
};

// Datos del jugador
struct infoJugador {
	int numVidas,
		danyoRecibido;

};

struct toDraw {
	Texture tex;
	SDL_Rect dest;
};