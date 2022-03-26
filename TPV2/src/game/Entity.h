#pragma once

#include <unordered_map>
#include <functional>
#include "PlayingState/FightManager.h"

class AnimationManager;



//TODO ESTO DEBERIA SALIR DE JSON TAMBIEN AAAAAAAAAA

struct animationData
{
	//Por cada animación...
	int iniSprite;		//En que frame empieza
	int totalSprites;	//Cuantos frames son
	int keySprite;		//Sprite de la animación en el que debe aparecer la hitbox (Con respecto al sprite inicial)
	int hitboxFrame;	//El frame en el que el juego crea la hitbox del ataque
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

struct attackData 
{
	b2Vec2 direction;
	int base;
	int damage;
	float multiplier;

	int startUp;
	int totalFrames;
};

struct OnHitData {
	int hitlag;
	bool zoom;
	bool bigHit;

	OnHitData() : 
		hitlag(0), zoom(false), bigHit(false) 
	{};

	OnHitData(int lag, bool zoomIn, bool bigEffect) :
		hitlag(lag), zoom(zoomIn), bigHit(bigEffect)
	{};
};

struct Hitbox {
	SDL_Rect box;
	attackData data;
	int duration;
	OnHitData hit;

	Hitbox(SDL_Rect a, attackData da, int frames, OnHitData b = OnHitData()) : 
		box(a), data(da), duration(frames), hit(b)
	{};
};

class Entity
{

protected:
	Texture* texture;

	FightManager* manager;

	SDLUtils* sdl;

	b2Body* body;

	SDL_Rect hurtbox;
	float width = 3.f;
	float height = 3.f;

	int dir;

	std::vector<Entity*> oponents;
	std::vector<Hitbox*> hitboxes;

	bool onGround;

	bool alive;
	int respawnTimer = 0;
	int respawnFrames;

	int lives = 3;
	Vector2D respawnPos;


public:


	Entity(FightManager* mngr, Vector2D* position, float w = 3.f, float h = 3.f);
	~Entity();

	virtual void update();
	virtual void draw();

	virtual void SetOponents(std::vector<Entity*> op);
	virtual void DeleteOponent(Entity* ent);

	virtual void CheckHits();
	virtual void OnDeath() { manager->RemoveEntity(this); };
	virtual bool GetHit(attackData a, int dir) = 0;

	FightManager* GetManager() { return manager; };

	virtual int GetDir() { return dir; };
	virtual float GetWidth() { return width; };
	virtual float GetHeight() { return height; };
	virtual SDL_Rect* GetHurtbox();
	virtual b2Body* GetBody() { return body; };
	void SetGround(bool ground);
	bool GetGround() { return onGround; };
	//virtual void SendToHUD(Texture* tex);
};