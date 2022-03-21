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
	std::function<void(int)> currentMove;

	// [x](int f) { x->TpAtack(f)}


//	void (Character::* currentMove)(int);
public:

	Character(FightManager* manager, Vector2D* pos, char input);
	~Character();

	virtual void update() override;
	virtual void draw() override;

	virtual bool GetHit(attackData a, int dir);
	virtual SDL_Rect* GetHurtbox();

	virtual void OnDeath() override;
	virtual void Respawn();

	virtual void BasicNeutral(int frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void BasicForward(int frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void BasicUpward(int frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void BasicDownward(int frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};

	virtual void SpecialNeutral(int frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void SpecialForward(int frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void SpecialUpward(int frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void SpecialDownward(int frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};


	virtual void StartShield(int frameNumber);
	virtual void EndShield(int frameNumber);
	virtual void Dash(int frameNumber);
};