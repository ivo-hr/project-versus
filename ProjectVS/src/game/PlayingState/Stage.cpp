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
	for (uint16 i = 0; i < grounds.size(); i++)
	{
		world->DestroyBody(grounds[i]);
	}
	for (uint16 i = 0; i < platforms.size(); i++)
	{
		world->DestroyBody(platforms[i]);
	}
	grounds.clear();
	groundRects.clear();
	platforms.clear();
	platformRects.clear();
	elements.clear();
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
	backGroundParallax = jsonFile["bgParallax"];


	auto images = jsonFile["elements"];
	assert(images.is_array());

	for (uint16 i = 0u; i < images.size(); i++)
	{
		auto info = images[i]["whereToDraw"];
		assert(info.is_array());

		for (uint16 j = 0u; j < info.size(); j++)
		{
			float par = info[j]["parallaxValue"];

			SDL_Rect rect = GetSDLCoors(info[j]["x"], info[j]["y"], info[j]["w"], info[j]["h"]);

			elements.push_back(TexWithRect(
				&sdl->images().at(images[i]["image"]),
				rect,
				par));
		}
	}

	auto groundData = jsonFile["groundData"];
	assert(groundData.is_array());

	for (uint16 i = 0u; i < groundData.size(); i++) {

		//Definimos un objeto (estatico)
		b2BodyDef groundDef;
		groundDef.position.Set(groundData[i]["groundX"], groundData[i]["groundY"]);
		groundDef.type = b2_staticBody;

		//Anadimos al mundo
		grounds.push_back(world->CreateBody(&groundDef));

		//Le damos forma...
		b2PolygonShape floor;
		float floorW = groundData[i]["groundW"], floorH = groundData[i]["groundH"];
		floor.SetAsBox(floorW / 2, floorH / 2);

		//..cuerpo
		b2FixtureDef fixt;
		fixt.shape = &floor;
		fixt.density = 10.0f;
		fixt.friction = 0.5f;
		fixt.filter.categoryBits = 2; // 2 para el suelo principal

		grounds[i]->CreateFixture(&fixt);

		groundRects.push_back(GetSDLCoors(grounds[i], floorW, floorH));
	}

	auto aData = jsonFile["platformData"];
	assert(aData.is_array());

	for (uint16 i = 0u; i < aData.size(); i++) {

		b2BodyDef gDef;
		gDef.position.Set(aData[i]["platformX"], aData[i]["platformY"] + 0.1f);
		gDef.type = b2_staticBody;

		b2PolygonShape plat;
		float platW = aData[i]["platformW"], platH = 0.2f;
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

	deathZone = { 0, 0, (int)(deathzoneSize.x * b2ToSDL), (int)(deathzoneSize.y * b2ToSDL) };

	auto player = jsonFile["playerSpawns"];
	assert(player.is_array());

	for (uint16 i = 0u; i < 4; i++)
	{
		playerSpawns.push_back(b2Vec2((float)player[i % 4]["X"], (float)player[i % 4]["Y"]));
	}
}

int Stage::GetPlayerDir(int index)
{
	return playerSpawns[index].x < deathzoneSize.x / 2 ? 1: -1 ;
}

void Stage::Update(SDL_Rect* camera)
{
	sdl->clearRenderer();

	SDL_Rect auxDeath = deathZone;

	float cameraWDiff = (float)mngr->GetActualWidth() - (float)camera->w;
	float cameraHDiff = (float)mngr->GetActualHeight() - (float)camera->h;

	float widthMat = (float)mngr->GetActualWidth() / ((float)camera->w + (cameraWDiff * (1.f - backGroundParallax)));
	float heightMat = (float)mngr->GetActualHeight() / ((float)camera->h + (cameraHDiff * (1.f - backGroundParallax)));

	auxDeath.x = (int)(-camera->x * backGroundParallax);
	auxDeath.x = (int)((float)auxDeath.x * widthMat);

	auxDeath.y = (int)(-camera->y * backGroundParallax);
	auxDeath.y = (int)((float)auxDeath.y * heightMat);

	auxDeath.w = (int)((float)auxDeath.w * widthMat);
	auxDeath.h = (int)((float)auxDeath.h * heightMat);

	background->render(auxDeath);

	for (TexWithRect aaa : elements)
	{
		SDL_Rect auxPlat = aaa.rect;

		widthMat = (float)mngr->GetActualWidth() / ((float)camera->w + (cameraWDiff * (1.f - aaa.parallaxValue)));
		heightMat = (float)mngr->GetActualHeight() / ((float)camera->h + (cameraHDiff * (1.f - aaa.parallaxValue)));

		auxPlat.x = auxPlat.x - (int)(camera->x * aaa.parallaxValue);
		auxPlat.x = (int)((float)auxPlat.x * widthMat);

		auxPlat.y = auxPlat.y - (int)(camera->y * aaa.parallaxValue);
		auxPlat.y = (int)((float)auxPlat.y * heightMat);

		auxPlat.w = (int)((float)auxPlat.w * widthMat);
		auxPlat.h = (int)((float)auxPlat.h * heightMat);

		aaa.image->render(auxPlat);
	}

#ifdef _DEBUG

	//Dibujamos las cajas
	SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);

	float wDiff = (float)mngr->GetActualWidth() / (float)camera->w;
	float hDiff = (float)mngr->GetActualHeight() / (float)camera->h;

	for (SDL_Rect aaa : groundRects)
	{
		SDL_Rect auxStage = aaa;

		auxStage.x -= camera->x;
		auxStage.x = (int)((float)auxStage.x * wDiff);

		auxStage.y -= camera->y;
		auxStage.y = (int)((float)auxStage.y * hDiff);

		auxStage.w = (int)((float)auxStage.w * wDiff);
		auxStage.h = (int)((float)auxStage.h * hDiff);

		SDL_RenderDrawRect(sdl->renderer(), &auxStage);
	}

	for (SDL_Rect aaa : platformRects)
	{
		SDL_Rect auxPlat = aaa;

		auxPlat.x -= camera->x;
		auxPlat.x = (int)((float)auxPlat.x * wDiff);

		auxPlat.y -= camera->y;
		auxPlat.y = (int)((float)auxPlat.y * hDiff);

		auxPlat.w = (int)((float)auxPlat.w * wDiff);
		auxPlat.h = (int)((float)auxPlat.h * hDiff);

		SDL_RenderDrawRect(sdl->renderer(), &auxPlat);
	}
	auxDeath = deathZone;

	auxDeath.x -= camera->x;
	auxDeath.x = (int)((float)auxDeath.x * wDiff);

	auxDeath.y -= camera->y;
	auxDeath.y = (int)((float)auxDeath.y * hDiff);

	auxDeath.w = (int)((float)auxDeath.w * wDiff);
	auxDeath.h = (int)((float)auxDeath.h * hDiff);

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