#include <vector>
#include <string>

#include "HUDManager.h"
#include "src/game/Entity.h"


HUDManager::HUDManager(float x, float y)
{

	datos.infoJugador.clear();


}

HUDManager::~HUDManager()
{

}

void HUDManager::AddPlayer(std::vector<Entity*> ent)
{
	infoJugador aux;

	for (int i = 0; i < ent.size(); ++i) {
		// aux.danyoRecibido = ent[i]->;
		aux.nombreJug = ent[i]->nombre;
		// aux.numVidas = ent[i].hitboxes;
		datos.infoJugador.push_back(aux);
	}
}

