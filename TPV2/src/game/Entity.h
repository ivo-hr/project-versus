#pragma once

#include <unordered_map>
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

	std::vector<animationData> animations;
	//std::unordered_map<std::string, animationData> animations;
};

struct attackData 
{
	b2Vec2 direction;
	int base;
	int damage;
	float multiplier;
};

class Entity
{

protected:

	spriteSheetData spData;
	AnimationManager* anim;

	Texture* texture;

	FightManager* manager;

	SDLUtils* sdl;

	b2Body* body;

	SDL_Rect hurtbox;
	float width = 3.f;
	float height = 3.f;

	int dir;

	std::vector<Entity*> oponents;

	bool onGround;

	bool alive;
	int respawnTimer = 0;
	int respawnFrames;

	int lives = 3;
	Vector2D respawnPos;


public:


	Entity(FightManager* mngr, Vector2D* position);
	~Entity();

	virtual void update();
	virtual void draw();

	virtual void SetOponents(std::vector<Entity*> op);

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