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

	// Adici�n de personaje x a la estructura de datos "datos"
	void AddPlayer(Character* character, gameData datos);
	// Eliminaci�n personaje x seg�n su posici�n en el vector infoJugadores
	void DeletePlayer(gameData datos, int pos);
	// Inclusi�n de personajes y sus datos en el vector infoJugadores
	void InitializePlayers(std::vector<Character*> characters, gameData datos);
	// Renderizaci�n de la interfaz mostrada en pantalla
	void RenderText(const std::string, SDL_Color fgColor);
	// Actualiza el da�o del personaje en cuesti�n
	void UpdateDamage(int damage, int pos, gameData datos);
	// Actualiza el n�mero de vidas del jugador
	void UpdateLives(int lives, int pos, gameData datos);
	// M�todo que renderiza de nuevo todo el HUD cuando se presenta un cambio
	void RenderAllHUD();
};