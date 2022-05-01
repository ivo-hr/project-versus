#pragma once

#include <string>
#include <SDL_ttf.h>
#include <vector>
#include <string>

#include "src/game/Character.h"
#include "src/sdlutils/Texture.h"
#include "src/sdlutils/SDLUtils.h"
#include "src/sdlutils/Font.h"


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
	std::vector<infoJugador> infoJugadores;
};

struct toDraw {
	Texture* tex;
	SDL_Rect dest;
};

class HUDManager
{
	Texture* tex;
	SDLUtils* sdl;

public:
	gameData datos;
	// toDraw dibujos;

	HUDManager(SDLUtils* sdl);
	~HUDManager();

	// Adición de personaje x a la estructura de datos "datos"
	void AddPlayer(Character* character, gameData datos);
	// Eliminación personaje x según su posición en el vector infoJugadores
	void DeletePlayer(gameData datos, int pos);
	// Inclusión de personajes y sus datos en el vector infoJugadores
	void InitializePlayers(std::vector<Character*> characters, gameData datos);
	// Renderización de la interfaz mostrada en pantalla
	void RenderText(const std::string, SDL_Color fgColor);
	// Actualiza el daño del personaje en cuestión
	void UpdateDamage(int damage, int pos, gameData datos);
	// Actualiza el número de vidas del jugador
	void UpdateLives(int lives, int pos, gameData datos);
	// Método que renderiza de nuevo todo el HUD cuando se presenta un cambio
	void RenderAllHUD();
};