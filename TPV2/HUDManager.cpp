#include <vector>
#include <string>

#include "HUDManager.h"


HUDManager::HUDManager(float x, float y) {
	gameData datos;

	int a = datos.numJugadores;

	// datos.infoJugador.push_back(datos.numJugadores);

	for (int i = 0; i < datos.numJugadores; ++i) {
		datos.infoJugador[i].nombreJug = "a";
		datos.infoJugador[i].danyoRecibido = 0;
		datos.infoJugador[i].numVidas = 3;
	}

	
}

HUDManager::~HUDManager()
{

}

