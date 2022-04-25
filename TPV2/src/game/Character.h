#pragma once

#include "Entity.h"
#include "../sdlutils/InputHandler.h"
#include "Utils/InputConfig.h"
#include "../json/json.hpp"
#include <fstream>
using json = nlohmann::json;

class Character : public Entity
{
protected:

	//InputHandler& ih = *InputHandler::instance();
	InputConfig *input = nullptr;

	spriteSheetData spData;

	AnimationManager* anim;

	//Datos de los ataques (Deberian salir de jsons en un futuro)
	std::unordered_map<std::string, attackData> attacks;

	int stun;
	int lives;

	string codeName;
	bool moving;
	int maxSpeed;
	int speed;
	bool shield;
	int shieldCounter;
	int maxShield;
	bool dash;
	//Variables para el salto (fuerza, maximo numero permitido, y el contador)
	int jumpStr;
	int maxJumps;
	int jumpCounter;
	bool jumpCooldown;
	int maxFallCount = 30; // Para iniciar los dos siguientes contadores:
	int fall = 0; // Contador entre pulsaciones de abajo (para bajar plataformas)
	int reactivateColl = 0; // Contador para reactivar colisiones con plataforma
	bool down = false; // El jugador pulsó abajo

	int weight;

	int damageTaken;

	bool recovery = true;

	//frame actual del movimiento que este haciendo
	int moveFrame = 0;

	bool alive = true;
	int respawnFrames = 150;
	//Metodo del movimiento que este haciendo (esto es una variable que guarda metodos :v)
	std::function<void(int)> currentMove;

	json ReadJson(std::string file);

	// efectos de estado
	enum state { none, fire, electric, water };

	state efEstado = none;

//	void (Character::* currentMove)(int);
public:

	Character(FightManager* manager, Vector2D* pos, char input, float w = 3.f, float h = 3.f);
	~Character();

	virtual void update() override;
	virtual void draw() override;
	virtual void draw(SDL_Rect* camera) override;

	virtual bool GetHit(attackData a, Entity* attacker);
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


	virtual void StartJump(int frameNumber);
	virtual void StartShield(int frameNumber);
	virtual void EndShield(int frameNumber);
	virtual void Dash(int frameNumber);

	void StartMove(std::function<void(int)> newMove);
	void ChangeMove(std::function<void(int)> newMove);
};