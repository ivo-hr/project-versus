#include "Stage.h"
#include "../Utils/MyListener.h"
#include "../../json/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Stage::Stage(SDLUtils* sdl, MyListener* _listener, double screenAdjust, float step, std::string filename):world(b2World(b2Vec2(0.f, 15.f))), sdl(sdl), step(step)
{
	std::ifstream file(filename);
	json jsonFile;
	file >> jsonFile;

	background = &sdl->images().at(jsonFile["background"]);
	platformTexture = &sdl->images().at(jsonFile["platform"]);

	//Definimos un objeto (estatico)
	b2BodyDef groundDef;
	groundDef.position.Set(jsonFile["groundX"], jsonFile["groundY"]);
	groundDef.type = b2_staticBody;

	//Anadimos al mundo
	stage = world.CreateBody(&groundDef);;
	//Le damos forma...
	b2PolygonShape floor;
	float floorW = jsonFile["groundW"], floorH = jsonFile["groundH"];
	floor.SetAsBox(floorW / 2, floorH / 2);

	//..cuerpo
	b2FixtureDef fixt;
	fixt.shape = &floor;
	fixt.density = 10.0f;
	fixt.friction = 0.5f;
	fixt.filter.categoryBits = 2; // 2 para el suelo principal
	fixt.filter.maskBits = 1; // Colisiona con los personajes (tienen este categoryBits en Entity)

	stage->CreateFixture(&fixt);

	b2BodyDef gDef;
	gDef.position.Set(jsonFile["platformX"], jsonFile["platformY"]);
	gDef.type = b2_staticBody;
	platform = world.CreateBody(&gDef);
	b2PolygonShape plat;
	float platW = jsonFile["platformW"], platH = jsonFile["platformH"];
	plat.SetAsBox(platW / 2, platH / 2);
	b2FixtureDef fi;
	fi.shape = &plat;
	fi.density = 10.0f;;
	fi.friction = 0.5f;
	fi.filter.categoryBits = 4; // 4 para las plataformas que puedes atravesar desde abajo
	fixt.filter.maskBits = 1; // Colisiona con los personajes (tienen este categoryBits en Entity)
	platform->CreateFixture(&fi);

	b2ToSDL = (sdl->width() * screenAdjust) / deathzoneSize;

	//Creo las cajas que representaran a los objetos
	stageRect = GetSDLCoors(stage, floorW, floorH);
	platformRect = GetSDLCoors(platform, platW, platH);

	listener = _listener;
	world.SetContactListener(listener);

	deathZone = { 0, 0, (int)(sdl->width() * screenAdjust), (int)(sdl->height() * screenAdjust) };

}
Stage::~Stage() 
{
	
}

void Stage::Update() 
{
	//Esto llama al mundo para que simule lo que pasa en el tiempo que se le pase (en este caso 1000.f/60.f (un frame a 60 fps))
	sdl->clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

	//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
	background->render(deathZone);
	platformTexture->render(platformRect);
	platformTexture->render(stageRect);
	//Dibujamos las cajas
	SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(sdl->renderer(), &stageRect);
	SDL_RenderDrawRect(sdl->renderer(), &platformRect);
	SDL_RenderDrawRect(sdl->renderer(), &deathZone);
}

SDL_Rect Stage::GetSDLCoors(b2Body* body, float width, float height) 
{
	return { (int)((body->GetPosition().x * b2ToSDL) - (width * b2ToSDL) / 2.f),
		(int)((body->GetPosition().y * b2ToSDL) - (height * b2ToSDL) / 2.f),
		(int)(width * b2ToSDL),
		(int)(height * b2ToSDL) };
}
SDL_Rect Stage::GetSDLCoors(float x, float y, float width, float height)
{
	return { (int)((x * b2ToSDL) - (width * b2ToSDL) / 2.f),
		(int)((y * b2ToSDL) - (height * b2ToSDL) / 2.f),
		(int)(width * b2ToSDL),
		(int)(height * b2ToSDL) };
}
int Stage::b2ToSDLX(b2Body* body, float width)
{
	return (int)(body->GetPosition().x * b2ToSDL - width * b2ToSDL / 2);
}

int Stage::b2ToSDLY(b2Body* body, float height)
{
	return (int)(body->GetPosition().y * b2ToSDL - height * b2ToSDL / 2);
}