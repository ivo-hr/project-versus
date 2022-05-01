#include <vector>
#include <string>

#include "HUDManager.h"
#include "src/game/Character.h"
#include "src/sdlutils/Texture.h"
#include "src/sdlutils/SDLUtils.h"


HUDManager::HUDManager(SDLUtils* sdl2)
{
	sdl = sdl2;

	datos.infoJugadores.clear();
	//dibujos.dest.x = x; dibujos.dest.y = y;
	//dibujos.tex->render(x, y);
}

HUDManager::~HUDManager()
{

}

void HUDManager::InitializePlayers(std::vector<Character*> characters, gameData datos)
{
	infoJugador aux;

	for (int i = 0; i < characters.size(); ++i) {
		aux.danyoRecibido = characters[i]->getDamageTaken();
		aux.nombreJug = characters[i]->codeName;
		aux.numVidas = characters[i]->getCurrentLives();
		datos.infoJugadores.push_back(aux);
	}

	datos.numJugadores = datos.infoJugadores.size();

	RenderAllHUD();
}

void HUDManager::UpdateDamage(int damage, int pos, gameData datos)
{
	datos.infoJugadores[pos].danyoRecibido = damage;
}

void HUDManager::UpdateLives(int lives, int pos, gameData datos)
{
	datos.infoJugadores[pos].numVidas = lives;
}

void HUDManager::RenderAllHUD()
{
	for (int i = 0; i < datos.numJugadores; ++i) {
		//RenderText("Daño recibido: " + datos.infoJugadores[i].danyoRecibido, );
		//RenderText("Jugador " + i, );
		//RenderText("Vidas restantes : " + datos.infoJugadores[i].numVidas, );
	}
}

void HUDManager::AddPlayer(Character* character, gameData datos) 
{
	infoJugador aux;

	aux.danyoRecibido = character->getDamageTaken();
	aux.nombreJug = character->codeName;
	aux.numVidas = character->getCurrentLives();
	datos.infoJugadores.push_back(aux);

	datos.numJugadores = datos.infoJugadores.size();
}

void HUDManager::DeletePlayer(gameData datos, int pos)
{
	for (int i = 0; i < datos.numJugadores - 1; ++i) {
		if (i >= pos) {
			datos.infoJugadores[i].danyoRecibido =
				datos.infoJugadores[i + 1].danyoRecibido;
			datos.infoJugadores[i].nombreJug =
				datos.infoJugadores[i + 1].nombreJug;
			datos.infoJugadores[i].numVidas =
				datos.infoJugadores[i + 1].numVidas;
		}
	}

	datos.numJugadores--;
	datos.infoJugadores.pop_back();

	RenderAllHUD();
}

void HUDManager::RenderText(const std::string texto, SDL_Color fgColor)
{
	string fontstring = "nes32";
	auto& font = sdl->fonts().at(fontstring);
	tex = new Texture(sdl->renderer(), texto, font, fgColor);
}