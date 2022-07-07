#pragma once

#include <unordered_map>
#include <functional>
#include "PlayingState/FightManager.h"

class AnimationManager;

enum state { none, fire, electric, water, wElectric };

struct animationData
{
	//Por cada animación...
	int iniSprite;		//En que frame empieza
	int totalSprites;	//Cuantos frames son
	std::vector<int>  keySprite = std::vector<int>();		//Sprite de la animación en el que debe aparecer la hitbox (Con respecto al sprite inicial)
	std::vector<int>  keyFrame = std::vector<int>();		//Los frames en el que el juego crea las hitbox de los ataques
	int totalFrames;	//Cuantos frames dura el movimiento en total (Hasta que se repita o acabe)
	bool loop;			//Si se debe loopear
};

struct spriteSheetData
{
	int leftOffset;		//Cuanto debe sobresalir la spritesheet a la izquierda (par que el dibujo coincida con la hurtbox)
	int upOffset;		//bruh
	int sizeXOffset;	//Que tan grande se tiene que hacer el sprite en X para que coincida
	int sizeYOffset;	//bruh

	int spritesInX;		//Cuantos frames hay en una fila de la spritesheet entera
	int spritesInY;		//Cuantos frames hay en una columna de la spritesheet entera

	std::unordered_map<std::string, animationData> animations;
	//std::unordered_map<std::string, animationData> animations;
};

struct HitData {

	int damage;

	b2Vec2 direction;
	int base;
	float multiplier;

	int stun = -1;
	int GetStun(float recoil) { return stun <= 0 ? (recoil / 1.8f) + 4 : stun; };

	bool shieldBreak = false;

	state estado = none;
	int power = 0;
};

struct HitBoxData
{
	SDL_Rect box;

	HitData hitdata;

	int hitlag = -1;
	int GetHitlag() { return hitlag <= 0 ? hitdata.damage * 0.9f : hitlag; };

	int duration;
	int outFor = 0;

	Vector2D charOffset;	//Offset que tiene con respecto a la entidad para que la siga
	Vector2D normalOffset;
};

struct attackData
{
	std::vector<HitBoxData> hitBoxes;

	std::vector<int> keyFrames;
	int totalFrames = 0;
};

class Entity
{

protected:
	Texture* texture;

	FightManager* manager;

	std::vector<Particle*> particulas;

	SDLUtils* sdl;

	b2Body* body;

	SDL_Rect hurtbox;
	float width = 3.f;
	float height = 3.f;

	int dir;

	std::vector<Entity*> oponents;
	std::vector<HitBoxData*> hitboxes;
	std::vector<bool> isHit;

	bool onGround;
	bool projectile = false;

	bool alive;
	int respawnTimer = 0;
	int respawnFrames;

	uint16 hitLag = 0;

	int lives = 3;
	b2Vec2 respawnPos;

	int kills;
	Entity* lastCharacter; // El ultimo jugador que golpeo a este

	bool toDelete = false;

public:
	string nombre;

	Entity(FightManager* mngr, b2Vec2 position, float w = 3.f, float h = 3.f);
	virtual ~Entity();

	virtual void updateParticles();
	virtual void update();
	virtual void draw();
	virtual void draw(SDL_Rect* camera);

	void AddParticle(Particle* par);
	bool RemoveParticle(Particle* par);

	virtual void SetOponents(std::vector<Entity*> op);
	virtual void AddOponent(Entity* ent);
	virtual void DeleteOponent(Entity* ent);

	virtual void CheckHits();
	virtual void OnDeath() { toDelete = true; };
	virtual bool GetHit(HitData a, Entity* attacker) { return false; };

	FightManager* GetManager() { return manager; };

	virtual int GetDir() { return dir; };
	virtual float GetWidth() { return width; };
	virtual float GetHeight() { return height; };
	virtual SDL_Rect* GetHurtbox();
	virtual b2Body* GetBody() { return body; };

	void AddHitLag(uint16 lag);

	Vector2D GetCenterSDL() { return Vector2D(hurtbox.x + (hurtbox.w / 2), hurtbox.y + (hurtbox.h / 2)); };

	void SetGround(bool ground);
	bool GetGround() { return onGround; };
	bool isProjectile() { return projectile; };

	virtual bool changeDir() { return false; };

	void resetHit();
	void increaseKills() { kills++; }
	int getKills() { return kills; }
	void setLastCharacer(Entity* chrcter);
	void resetLastCharacter() { lastCharacter = nullptr; }

	Texture* getTexture() { return texture; }
	//virtual void SendToHUD(Texture* tex);
	bool ToDelete() { return toDelete; };
	virtual bool isCharacter() { return false; };
};