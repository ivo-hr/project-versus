#pragma once

#include <string>

#include "../../../box2d/include/box2d/box2d.h"
#include <SDL.h>
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

struct atackData {
	int damage;
	float multiplier;
};

class Character
{
protected:

	int dir = 1;

	bool movable;

	SDLUtils* sdl;
	Character* oponent = nullptr;

	b2Body* body;

	SDL_Rect hurtbox;
	float width = 5.f;
	float height = 5.f;

	InputHandler& ih = *InputHandler::instance();

	//Datos de los ataques (Deberian salir de jsons en un futuro)
	atackData ataqueFuerte;
	atackData ataqueDebil;

	bool moving;
	int maxSpeed;
	int speed;

	//Variables para el salto (fuerza, maximonumero permitido, y el contador)
	int jumpStr;
	int maxJumps;
	int jumpCounter;

	int weight;

	int damageTaken;

	//frame actual del movimiento que este haciendo
	int moveFrame;
	//Metodo del movimiento que este haciendo (esto es una variable que guarda metodos :v)
	void (Character::* currentMove)(int);
public:

	Character(b2World* world, SDLUtils* sdl, bool movable);
	~Character();

	virtual void update();
	virtual void atackWeak(int frameNumber);
	virtual void atackStrong(int frameNumber);
	virtual void draw();

	virtual void GetHit(atackData a, int dir);
	virtual void SetOponent(Character* op);
	virtual SDL_Rect* GetHurtbox();

};