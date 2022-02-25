#pragma once

#include "Entity.h"
#include "../sdlutils/InputHandler.h"

class Character : public Entity
{
protected:

	int dir = 1;

	bool movable;

	Character* oponent = nullptr;

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
	int moveFrame = 0;
	//Metodo del movimiento que este haciendo (esto es una variable que guarda metodos :v)
	void (Character::* currentMove)(int);
public:

	Character(b2World* world, SDLUtils* sdl, bool movable, SDL_Texture* texture);
	~Character();

	virtual void update() override;
	virtual void atackWeak(int frameNumber);
	virtual void atackStrong(int frameNumber);
	virtual void draw() override;

	virtual void GetHit(atackData a, int dir);
	virtual void SetOponent(Character* op);
	virtual SDL_Rect* GetHurtbox();
	bool GetGround() { return onGround; };
	void SetGround();

};