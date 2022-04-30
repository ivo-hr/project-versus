#include "Stage.h"
#include "../Utils/MyListener.h"
#include "../../json/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void Stage::reset()
{

}

Stage::Stage(SDLUtils* sdl, MyListener* _listener, float step) :
	world(new b2World(b2Vec2(0.f, 15.f))), sdl(sdl), step(step)
{
	listener = _listener;
}
Stage::~Stage() 
{
	UnLoadStage();
}

void Stage::UnLoadStage()
{
	world->DestroyBody(stage);
	for (uint16 i = 0; i < platforms.size(); i++)
	{
		world->DestroyBody(platforms[i]);
	}
	platforms.clear();
	platformRects.clear();
	playerSpawns.clear();
}

void Stage::LoadJsonStage(std::string fileName, double screenAdjust)
{
	std::ifstream file(fileName);
	json jsonFile;
	file >> jsonFile;

	deathzoneSize = jsonFile["deathZoneSize"];

	b2ToSDL = (sdl->width() * screenAdjust) / deathzoneSize;

	background = &sdl->images().at(jsonFile["background"]);
	platformTexture = &sdl->images().at(jsonFile["platform"]);

	//Definimos un objeto (estatico)
	b2BodyDef groundDef;
	groundDef.position.Set(jsonFile["groundX"], jsonFile["groundY"]);
	groundDef.type = b2_staticBody;

	//Anadimos al mundo
	stage = world->CreateBody(&groundDef);;
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

	auto aData = jsonFile["platformData"];
	assert(aData.is_array());

	for (uint16 i = 0u; i < aData.size(); i++) {

		b2BodyDef gDef;
		gDef.position.Set(aData[i]["platformX"], aData[i]["platformY"]);
		gDef.type = b2_staticBody;

		b2PolygonShape plat;
		float platW = aData[i]["platformW"], platH = aData[i]["platformH"];
		plat.SetAsBox(platW / 2, platH / 2);

		b2FixtureDef fi;
		fi.shape = &plat;
		fi.density = 10.0f;;
		fi.friction = 0.5f;
		fi.filter.categoryBits = 4; // 4 para las plataformas que puedes atravesar desde abajo
		fixt.filter.maskBits = 1; // Colisiona con los personajes (tienen este categoryBits en Entity)

		platforms.push_back(world->CreateBody(&gDef));
		platforms[i]->CreateFixture(&fi);

		platformRects.push_back(GetSDLCoors(platforms[i], platW, platH));
	}

	//Creo las cajas que representaran a los objetos
	stageRect = GetSDLCoors(stage, floorW, floorH);

	world->SetContactListener(listener);

	deathZone = { 0, 0, (int)(sdl->width() * screenAdjust), (int)(sdl->height() * screenAdjust) };

	auto player = jsonFile["playerSpawns"];
	assert(player.is_array());

	for (uint16 i = 0u; i < 4; i++)
	{
		playerSpawns.push_back(b2Vec2(player[i % 4]["X"], player[i % 4]["Y"]));
	}
}

int Stage::GetPlayerDir(int index)
{
	return playerSpawns[index].x < deathzoneSize / 2 ?1: -1 ;
}

void Stage::Update()
{
	sdl->clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

	//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
	background->render(deathZone);

	for (SDL_Rect aaa : platformRects)
	{
		platformTexture->render(aaa);
	}
	platformTexture->render(stageRect);

#ifdef _DEBUG
	SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(sdl->renderer(), &stageRect);

	for (SDL_Rect aaa : platformRects)
	{
		SDL_RenderDrawRect(sdl->renderer(), &aaa);
	}

	SDL_RenderDrawRect(sdl->renderer(), &deathZone);
#endif
}

void Stage::Update(SDL_Rect* camera)
{
	sdl->clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

	SDL_Rect auxDeath = deathZone;

	auxDeath.w += auxDeath.w * 0.5f;
	auxDeath.h += auxDeath.h * 0.5f;

	Vector2D camCenter = Vector2D(camera->x + camera->w / 2, camera->y + camera->h / 2);

	auxDeath.x = (camCenter.getX() - deathZone.w / 2) * 0.5f;
	auxDeath.x += deathZone.w / 2;
	auxDeath.x -= (auxDeath.w * 0.5f);

	auxDeath.y = (camCenter.getY() - deathZone.h / 2) * -0.2f;
	auxDeath.y += deathZone.h / 2;
	auxDeath.y -= (auxDeath.h * 0.5f);

	background->render(auxDeath);

	for (SDL_Rect aaa : platformRects)
	{
		SDL_Rect auxPlat = aaa;

		auxPlat.x -= camera->x;
		auxPlat.x *= (deathZone.w / (float)camera->w);

		auxPlat.y -= camera->y;
		auxPlat.y *= (deathZone.h / (float)camera->h);

		auxPlat.w *= (deathZone.w / (float)camera->w);
		auxPlat.h *= (deathZone.h / (float)camera->h);

		platformTexture->render(auxPlat);
	}

	SDL_Rect auxStage = stageRect;

	auxStage.x -= camera->x;
	auxStage.x *= (deathZone.w / (float)camera->w);

	auxStage.y -= camera->y;
	auxStage.y *= (deathZone.h / (float)camera->h);

	auxStage.w *= (deathZone.w / (float)camera->w);
	auxStage.h *= (deathZone.h / (float)camera->h);

	platformTexture->render(auxStage);

#ifdef _DEBUG

	//Dibujamos las cajas
	SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(sdl->renderer(), &auxStage);

	for (SDL_Rect aaa : platformRects)
	{
		SDL_Rect auxPlat = aaa;

		auxPlat.x -= camera->x;
		auxPlat.x *= (deathZone.w / (float)camera->w);

		auxPlat.y -= camera->y;
		auxPlat.y *= (deathZone.h / (float)camera->h);

		auxPlat.w *= (deathZone.w / (float)camera->w);
		auxPlat.h *= (deathZone.h / (float)camera->h);

		SDL_RenderDrawRect(sdl->renderer(), &auxPlat);
	}
	auxDeath = deathZone;
	auxDeath.x -= camera->x;
	auxDeath.x *= (deathZone.w / (float)camera->w);

	auxDeath.y -= camera->y;
	auxDeath.y *= (deathZone.h / (float)camera->h);

	auxDeath.w *= (deathZone.w / (float)camera->w);
	auxDeath.h *= (deathZone.h / (float)camera->h);
	SDL_RenderDrawRect(sdl->renderer(), &auxDeath);

#endif // _DEBUG
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