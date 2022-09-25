#include "Stage.h"
#include "../Utils/MyListener.h"
#include "../../json/json.hpp"
#include "../../utils/CheckML.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void Stage::reset()
{

}

Stage::Stage(FightManager* mngr, SDLUtils* sdl, MyListener* _listener, float step) :
	mngr(mngr), world(new b2World(b2Vec2(0.f, 13.f))), sdl(sdl), step(step)
{
	listener = _listener;

	world->SetContactListener(listener);
}
Stage::~Stage() 
{
	delete world;
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

void Stage::LoadJsonStage(std::string fileName, int width, int height)
{
	std::ifstream file(fileName);
	json jsonFile;
	file >> jsonFile;

	deathzoneSize.x = jsonFile["deathZoneSizeX"];
	deathzoneSize.y = jsonFile["deathZoneSizeY"];

	b2ToSDL = width / deathzoneSize.x;

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

		platforms.push_back(world->CreateBody(&gDef));
		platforms[i]->CreateFixture(&fi);

		platformRects.push_back(GetSDLCoors(platforms[i], platW, platH));
	}

	//Creo las cajas que representaran a los objetos
	stageRect = GetSDLCoors(stage, floorW, floorH);

	deathZone = { 0, 0, (int)(deathzoneSize.x * b2ToSDL), (int)(deathzoneSize.y * b2ToSDL) };

	auto player = jsonFile["playerSpawns"];
	assert(player.is_array());

	for (uint16 i = 0u; i < 4; i++)
	{
		playerSpawns.push_back(b2Vec2(player[i % 4]["X"], player[i % 4]["Y"]));
	}
}

int Stage::GetPlayerDir(int index)
{
	return playerSpawns[index].x < deathzoneSize.x / 2 ? 1: -1 ;
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

	float parallaxValue = 0.2f;

	float cameraWDiff = (float)mngr->GetActualWidth() - (float)camera->w;
	float cameraHDiff = (float)mngr->GetActualHeight() - (float)camera->h;

	auxDeath.x = (auxDeath.x - camera->x) * parallaxValue;
	auxDeath.x *= ((float)mngr->GetActualWidth() / ((float)camera->w + (cameraWDiff * (1.f - parallaxValue))));

	auxDeath.y = (auxDeath.y - camera->y) * parallaxValue;
	auxDeath.y *= ((float)mngr->GetActualHeight() / ((float)camera->h + (cameraHDiff * (1.f - parallaxValue))));

	auxDeath.w *= ((float)mngr->GetActualWidth() / ((float)camera->w + (cameraWDiff * (1.f - parallaxValue))));
	auxDeath.h *= ((float)mngr->GetActualHeight() / ((float)camera->h + (cameraHDiff * (1.f - parallaxValue))));

	background->render(auxDeath);

	for (SDL_Rect aaa : platformRects)
	{
		SDL_Rect auxPlat = aaa;

		auxPlat.x -= camera->x;
		auxPlat.x *= (mngr->GetActualWidth() / (float)camera->w);

		auxPlat.y -= camera->y;
		auxPlat.y *= (mngr->GetActualWidth() / (float)camera->w);

		auxPlat.w *= (mngr->GetActualWidth() / (float)camera->w);
		auxPlat.h *= (mngr->GetActualWidth() / (float)camera->w);

		platformTexture->render(auxPlat);
	}

	SDL_Rect auxStage = stageRect;

	auxStage.x -= camera->x;
	auxStage.x *= (mngr->GetActualWidth() / (float)camera->w);

	auxStage.y -= camera->y;
	auxStage.y *= (mngr->GetActualWidth() / (float)camera->w);

	auxStage.w *= (mngr->GetActualWidth() / (float)camera->w);
	auxStage.h *= (mngr->GetActualWidth() / (float)camera->w);

	platformTexture->render(auxStage);

#ifdef _DEBUG

	//Dibujamos las cajas
	SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(sdl->renderer(), &auxStage);

	for (SDL_Rect aaa : platformRects)
	{
		SDL_Rect auxPlat = aaa;

		auxPlat.x -= camera->x;
		auxPlat.x *= (mngr->GetActualWidth() / (float)camera->w);

		auxPlat.y -= camera->y;
		auxPlat.y *= (mngr->GetActualWidth() / (float)camera->w);

		auxPlat.w *= (mngr->GetActualWidth() / (float)camera->w);
		auxPlat.h *= (mngr->GetActualWidth() / (float)camera->w);

		SDL_RenderDrawRect(sdl->renderer(), &auxPlat);
	}
	auxDeath = deathZone;
	auxDeath.x -= camera->x;
	auxDeath.x *= (mngr->GetActualWidth() / (float)camera->w);

	auxDeath.y -= camera->y;
	auxDeath.y *= (mngr->GetActualWidth() / (float)camera->w);

	auxDeath.w *= (mngr->GetActualWidth() / (float)camera->w);
	auxDeath.h *= (mngr->GetActualWidth() / (float)camera->w);
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