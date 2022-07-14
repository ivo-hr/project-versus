#pragma once

#include "Entity.h"
#include "../sdlutils/InputHandler.h"
#include "Utils/InputConfig.h"
#include "../json/json.hpp"
#include <fstream>
#include "Utils/Particle.h"

using json = nlohmann::json;

class Character : public Entity
{
protected:
	uint16 playerNumber;
	int playerPosition;//P1 ,P2 ,P3,P4..
	//InputHandler& ih = *InputHandler::instance();
	InputConfig *input = nullptr;

	AnimationManager* anim;

	//Datos de los ataques (Deberian salir de jsons en un futuro)
	std::unordered_map<std::string, attackData> attacks;

	Texture* arrowsTex;
	Texture* portrait;
	SDL_Rect arrowSrc;

	int stun;
	int lives;

	const int maxLives = 3;

	string codeName;
	bool moving;
	int maxSpeed;
	int speed;
	uint16 shield;
	uint16 shieldHealth;
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
	int totalDamageTaken;

	
	bool recovery = true;

	//frame actual del movimiento que este haciendo
	int moveFrame = 0;

	bool alive = true;
	int respawnFrames = 150;
	//Metodo del movimiento que este haciendo (esto es una variable que guarda metodos :v)
	std::function<void(int)> currentMove;

	json ReadJson(std::string file, spriteSheetData& spData);

	// efectos de estado
	enum state efEstado = none;
	int statePower = 0;
	int stateCont = 0;
	int stateDur = 300;
	double ralentizar = 0;
//	void (Character::* currentMove)(int);
	int input_;

	int r = 0;
	int g = 255;

	int arrowCont = 0;
	bool invencible = false;
	bool drawArrow=false;
	int invencibleCont = 0;

	std::string animAddon = "";

	virtual void CreateHitBox(HitBoxData* data);

	virtual void BuildBoxes() = 0;
	virtual Vector2D BuildBoxOffset(const HitBoxData& data) {
		return Vector2D((data.box.x + (data.box.w / 2)) - (hurtbox.x + (hurtbox.w / 2)), (data.box.y + (data.box.h / 2)) - (hurtbox.y + (hurtbox.h / 2)));
	};

public:

	Character(FightManager* manager, b2Vec2 pos, char input,int player, float w = 3.f, float h = 3.f);
	virtual ~Character();

	void SetSpawn(b2Vec2 spawn, int dir);
	void SetPNumber(uint16 num);

	virtual void update() override;
	void UpdateAnimations();
	void AllowAttack();
	void AllowMovement(bool changeDirection = false, bool showParticles = false);
	void AllowMovement(float multiplier, bool changeDirection = false, bool showParticles = false);
	void StunBehaviour();
	void DropPlatform();
	virtual void draw() override;
	virtual void draw(SDL_Rect* camera) override;
	virtual void drawHUD(int numOfPlayer) ;

	virtual bool GetHit(HitData a, Entity* attacker) override;
	virtual SDL_Rect* GetHurtbox();
	Texture* getPortrait() { return portrait; };

	virtual void OnDeath() override;
	void AddDeathParticle();
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
	virtual void DashLanding(int frameNumber);
	virtual void Taunt(int frameNumber);
	virtual void Elements();

	void StartMove(std::function<void(int)> newMove);
	void ChangeMove(std::function<void(int)> newMove);

	int getInput() { return input_; }
	int getDamageTaken() { return totalDamageTaken; }
	int getDeaths() { return maxLives - lives; }
	virtual bool isCharacter() override { return true; };
};