#pragma once

#include <unordered_map>
#include <queue>
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
	ushort leftOffset = 0;		//Cuanto debe sobresalir la spritesheet a la izquierda (par que el dibujo coincida con la hurtbox)
	ushort upOffset = 0;		//bruh
	ushort sizeXOffset = 0;	//Que tan grande se tiene que hacer el sprite en X para que coincida
	ushort sizeYOffset = 0;	//bruh

	ushort spritesInX = 0;		//Cuantos frames hay en una fila de la spritesheet entera
	ushort spritesInY = 0;		//Cuantos frames hay en una columna de la spritesheet entera

	std::unordered_map<std::string, animationData> animations = std::unordered_map<std::string, animationData>();
	//std::unordered_map<std::string, animationData> animations;
};

struct HitData {

	ushort damage;

	b2Vec2 direction;
	ushort base;
	float multiplier;

	short stun = -1;
	ushort GetStun(float recoil) { return stun <= 0 ? (ushort)(recoil / 1.8f) + 4 : stun; };

	bool shieldBreak = false;

	state estado = state::none;
	ushort power = 0;
};

struct HitBoxData
{
	SDL_Rect box;

	HitData hitdata;

	short hitlag = -1;
	ushort GetHitlag() { return hitlag <= 0 ? (ushort)((float)hitdata.damage * 0.9f) : hitlag; };

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

struct ParticleData
{
	Texture* tex = nullptr;

	SDL_Rect dest = { 0,0,0,0 };

	ushort numSprites = 0;
	ushort numSpritesinX = 0;
	ushort numSpritesinY = 0;

	ushort duration = 0;

	short dir = 1;

	ParticleData() {}

	ParticleData(Texture* im, SDL_Rect dest, ushort numS, ushort numX, ushort numY, ushort dur) :
		tex(im), dest(dest), numSprites(numS), numSpritesinX(numX), numSpritesinY(numY), duration(dur), dir(1)
	{ }
};

class Entity abstract
{
private:

	unordered_map<Tags, bool> tags = unordered_map<Tags, bool>({ {Tags::CameraFollow, false}, {Tags::IsCharacter, false}, {Tags::IsProjectile, false}, {Tags::Hitable, false} });

protected:

	bool alive = true;

	ushort layer;
	ushort placeInLayer;

	Texture* texture;

	FightManager* manager;

	SDLUtils* sdl;

	b2Body* body;

	SDL_Rect hurtbox;
	float width = 3.f;
	float height = 3.f;

	short dir;

	map<string, deque<Particle*>> particlePool;
	std::vector<Particle*> backParticles;
	std::vector<Particle*> frontParticles;

	std::vector<HitBoxData*> hitboxes;
	std::unordered_map<Entity*, bool> isHit;

	bool onGround;
	ushort respawnTimer = 0;
	ushort respawnFrames;

	ushort hitLag = 0;

	ushort lives = 3;
	b2Vec2 respawnPos;

	ushort kills;
	Entity* lastCharacter; // El ultimo jugador que golpeo a este

	Vector2D shakeValue;

	bool toDelete = false;

	virtual void BuildParticlePool();

public:
	Entity(FightManager* mngr, b2Vec2 position, float w = 3.f, float h = 3.f);
	virtual ~Entity();

	virtual string GetName() = 0;

	bool IsAlive() { return alive; }
	void SetAlive(bool is) { alive = is; }

	void AddTag(const Tags& tag) { tags[tag] = true; };
	void RemoveTag(const Tags& tag) { tags[tag] = false; };
	bool HasTag(const Tags& tag) { return tags[tag]; };

	virtual void SetLayer(ushort layer) { this->layer = layer; }
	ushort GetLayer() { return layer; }
	void SetPlaceInLayer(ushort placeInLayer) { this->placeInLayer = placeInLayer; }
	ushort GetPlaceInLayer() { return placeInLayer; }

	virtual void updateParticles();
	virtual void update();
	virtual void draw();
	virtual void draw(SDL_Rect* camera);

	void AddParticle(const string& name, const Vector2D& pos, short dir = 1, bool front = false);
	void RemoveParticle(Particle* par, ushort posInVec, bool front);

	virtual void CheckHits() { };
	virtual void OnDeath() { toDelete = true; };
	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) { return false; };

	FightManager* GetManager() { return manager; };

	virtual short GetDir() { return dir; };
	ushort GetLives() { return lives; };
	virtual float GetWidth() { return width; };
	virtual float GetHeight() { return height; };
	virtual SDL_Rect* GetHurtbox();
	virtual b2Body* GetBody() { return body; };

	virtual void SetPosition(const b2Vec2& newPos);

	void AddHitLag(ushort lag);

	Vector2D GetCenterSDL() { return Vector2D(hurtbox.x + ((float)hurtbox.w / 2.f), hurtbox.y + ((float)hurtbox.h / 2.f)); };

	void SetGround(bool ground);
	bool GetGround() { return onGround; };

	virtual bool IsOutOfBounds() { return !SDL_HasIntersection(&hurtbox, manager->GetDeathZone()); }

	virtual bool changeDir() { return false; };

	void resetHit();
	void increaseKills() { kills++; }
	int getKills() { return kills; }
	void setLastCharacer(Entity* chrcter);
	void resetLastCharacter() { lastCharacter = nullptr; }

	Texture* getTexture() { return texture; }
	virtual SDL_Rect getCurrentSpriteSrc() { return { 0, 0, texture->width(), texture->height() }; }
	//virtual void SendToHUD(Texture* tex);
	bool ToDelete() { return toDelete; };

	void SetShake(Vector2D dir, ushort value);
};