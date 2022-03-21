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
	attackData ataqueFuerte;
	attackData ataqueDebil;

	int stun;
	int lives;

	bool moving;
	int maxSpeed;
	int speed;
	bool shield;
	int shieldCounter;
	int maxShield;
	bool dash;

	//Variables para el salto (fuerza, maximonumero permitido, y el contador)
	int jumpStr;
	int maxJumps;
	int jumpCounter;

	int weight;

	int damageTaken;

	//frame actual del movimiento que este haciendo
	int moveFrame = 0;

	bool alive = true;
	int respawnFrames = 150;
	//Metodo del movimiento que este haciendo (esto es una variable que guarda metodos :v)
	void (Character::* currentMove)(int);
public:

	Character(FightManager* manager, Vector2D* pos, char input);
	~Character();

	virtual void update() override;
	virtual void draw() override;

	virtual bool GetHit(attackData a, int dir);
	virtual SDL_Rect* GetHurtbox();

	virtual void OnDeath() override;
	virtual void Respawn();

	virtual void BasicNeutral(int frameNumber) = 0;
	virtual void BasicForward(int frameNumber) = 0;
	virtual void BasicUpward(int frameNumber) = 0;
	virtual void BasicDownward(int frameNumber) = 0;

	virtual void SpecialNeutral(int frameNumber) = 0;
	virtual void SpecialForward(int frameNumber) = 0;
	virtual void SpecialUpward(int frameNumber) = 0;
	virtual void SpecialDownward(int frameNumber) = 0;

	virtual void Recovery(int frameNumber) = 0;

	virtual void StartShield(int frameNumber);
	virtual void EndShield(int frameNumber);
	virtual void Dash(int frameNumber);
};