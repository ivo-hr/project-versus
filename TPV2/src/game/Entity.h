#pragma once

#include <unordered_map>
#include <functional>
#include "PlayingState/FightManager.h"

class AnimationManager;

enum state { none, fire, electric, water, wElectric };

struct animationData
{
	//Por cada animación...
	ushort iniSprite;		//En que frame empieza
	ushort totalSprites;	//Cuantos frames son
	std::vector<ushort> keySprite = std::vector<ushort>();		//Sprite de la animación en el que debe aparecer la hitbox (Con respecto al sprite inicial)
	std::vector<ushort> keyFrame = std::vector<ushort>();		//Los frames en el que el juego crea las hitbox de los ataques
	ushort totalFrames;	//Cuantos frames dura el movimiento en total (Hasta que se repita o acabe)
	bool loop;			//Si se debe loopear
};

struct spriteSheetData
{
	ushort leftOffset;		//Cuanto debe sobresalir la spritesheet a la izquierda (par que el dibujo coincida con la hurtbox)
	ushort upOffset;		//bruh
	ushort sizeXOffset;	//Que tan grande se tiene que hacer el sprite en X para que coincida
	ushort sizeYOffset;	//bruh

	ushort spritesInX;		//Cuantos frames hay en una fila de la spritesheet entera
	ushort spritesInY;		//Cuantos frames hay en una columna de la spritesheet entera

	std::unordered_map<std::string, animationData> animations;
	//std::unordered_map<std::string, animationData> animations;
};

struct HitData {

	ushort damage;

	b2Vec2 direction;
	ushort base;
	float multiplier;

	short stun = -1;
	ushort GetStun(float recoil) { return stun <= 0 ? (recoil / 1.8f) + 4 : stun; };

	bool shieldBreak = false;

	state estado = none;
	ushort power = 0;
};

struct HitBoxData
{
	SDL_Rect box;

	HitData hitdata;

	short hitlag = -1;
	ushort GetHitlag() { return hitlag <= 0 ? hitdata.damage * 0.9f : hitlag; };

	ushort duration;
	ushort outFor = 0;

	Vector2D charOffset;	//Offset que tiene con respecto a la entidad para que la siga
	Vector2D normalOffset;
};

struct attackData
{
	std::vector<HitBoxData> hitBoxes;

	std::vector<ushort> keyFrames;
	ushort totalFrames = 0;
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

	short dir;

	std::vector<Entity*> oponents;
	std::vector<HitBoxData*> hitboxes;
	std::vector<bool> isHit;

	bool onGround;
	bool projectile = false;

	bool alive;
	ushort respawnTimer = 0;
	ushort respawnFrames;

	ushort hitLag = 0;

	ushort lives = 3;
	b2Vec2 respawnPos;

	ushort kills;
	Entity* lastCharacter; // El ultimo jugador que golpeo a este

	Vector2D shakeValue;

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
	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) { return false; };

	FightManager* GetManager() { return manager; };

	virtual short GetDir() { return dir; };
	virtual float GetWidth() { return width; };
	virtual float GetHeight() { return height; };
	virtual SDL_Rect* GetHurtbox();
	virtual b2Body* GetBody() { return body; };

	void AddHitLag(ushort lag);

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

	void SetShake(Vector2D dir, ushort value);
};