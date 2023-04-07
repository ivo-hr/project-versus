#pragma once

#include <unordered_map>
#include "../Entity.h"

class Entity;
class Fightmanager;

class Particle
{

protected:

	FightManager* manager;

	SDL_Rect src;

	ParticleData data;

	int spriteDuration;
	int cont;

	bool active;

	void CalcularResto();

public:

	Particle(const Vector2D& position, const std::string& id, Entity* ent, int dir = 1);
	Particle(const Vector2D& position, const ParticleData& data, Entity* ent);
	virtual ~Particle();

	void Activate(const Vector2D& pos, short dir = 1, bool isRun = false);
	void Deactivate();

	virtual void update();
	virtual void draw();
	virtual void draw(const SDL_Rect& camera);

	bool IsActive() { return active; };

	bool dead = false;
};