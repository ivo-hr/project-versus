#pragma once

#include <unordered_map>
#include <functional>
#include "../PlayingState/FightManager.h"
#include "../Entity.h"

class Entity;
class Fightmanager;

struct ParticleData
{
	Texture* tex;

	SDL_Rect dest;

	int numSprites;
	int numSpritesinX;
	int numSpritesinY;

	int duration;

	int dir;
};

class Particle
{

protected:

	FightManager* manager;

	Texture* texture;

	SDL_Rect src;

	ParticleData data;

	int spriteDuration;
	int cont;

	bool active;

	void CalcularResto();

public:

	Particle(const Vector2D& position, const std::string& id, Entity* ent, int dir);
	Particle(const Vector2D& position, const ParticleData& data, Entity* ent);
	virtual ~Particle();

	void Activate();
	void Deactivate();

	virtual void update();
	virtual void draw();
	virtual void draw(SDL_Rect* camera);

	bool dead = false;
};