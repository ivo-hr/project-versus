#pragma once

#include "Entity.h"
#include "../sdlutils/InputHandler.h"
#include "Utils/InputConfig.h"

class Character : public Entity
{
protected:

	//InputHandler& ih = *InputHandler::instance();
	InputConfig *input = nullptr;
	//Datos de los ataques (Deberian salir de jsons en un futuro)
	atackData ataqueFuerte;
	atackData ataqueDebil;

	int stun;

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

	Character(FightManager* manager, char input);
	~Character();

	virtual void update() override;
	virtual void draw() override;

	virtual void GetHit(atackData a, int dir);
	virtual SDL_Rect* GetHurtbox();
	bool GetGround() { return onGround; };
	void SetGround();

	virtual void BasicNeutral(int frameNumber) = 0;
	virtual void BasicForward(int frameNumber) = 0;
	virtual void BasicUpward(int frameNumber) = 0;
	virtual void BasicDownward(int frameNumber) = 0;

	virtual void SpecialNeutral(int frameNumber) = 0;
	virtual void SpecialForward(int frameNumber) = 0;
	virtual void SpecialUpward(int frameNumber) = 0;
	virtual void SpecialDownward(int frameNumber) = 0;

	
};