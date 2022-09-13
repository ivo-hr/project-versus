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
	ushort playerNumber;
	ushort playerPosition;//P1 ,P2 ,P3,P4..
	//InputHandler& ih = *InputHandler::instance();
	InputConfig *input = nullptr;

	AnimationManager* anim;

	//Datos de los ataques (Deberian salir de jsons en un futuro)
	std::unordered_map<std::string, attackData> attacks;

	Texture* arrowsTex;
	Texture* portrait;
	SDL_Rect arrowSrc;
	ushort stun;
	ushort lives;

	const ushort maxLives = 3;

	string codeName;
	bool moving;
	ushort maxSpeed;
	float speed;

	ushort shield;
	ushort shieldHealth;
	int maxShield;
	bool dash;
	//Variables para el salto (fuerza, maximo numero permitido, y el contador)
	ushort jumpStr;
	ushort maxJumps;
	ushort jumpCounter;
	bool jumpCooldown;
	ushort maxFallCount = 30; // Para iniciar los dos siguientes contadores:
	ushort fall = 0; // Contador entre pulsaciones de abajo (para bajar plataformas)
	ushort reactivateColl = 0; // Contador para reactivar colisiones con plataforma
	bool down = false; // El jugador pulsó abajo

	ushort weight;

	ushort damageTaken;
	ushort totalDamageTaken;

	
	bool recovery = true;

	//frame actual del movimiento que este haciendo
	short moveFrame = 0;

	bool alive = true;
	int respawnFrames = 150;
	//Metodo del movimiento que este haciendo (esto es una variable que guarda metodos :v)
	std::function<void(int)> currentMove;

	json ReadJson(std::string file, spriteSheetData& spData);

	// efectos de estado
	enum state efEstado = none;
	ushort statePower = 0;
	ushort stateCont = 0;
	ushort stateDur = 300;
	float ralentizar = 0;
//	void (Character::* currentMove)(int);
	ushort input_;

	ushort r = 0;
	ushort g = 255;

	unsigned int arrowCont = 0;
	bool invencible = false;
	bool drawArrow = false;
	unsigned int invencibleCont = 0;

	std::string animAddon = "";

	virtual void CreateHitBox(HitBoxData* data);

	virtual void BuildBoxes() = 0;
	virtual Vector2D BuildBoxOffset(const HitBoxData& data) {
		return Vector2D((data.box.x + (data.box.w / 2)) - (hurtbox.x + (hurtbox.w / 2)), (data.box.y + (data.box.h / 2)) - (hurtbox.y + (hurtbox.h / 2)));
	};

public:

	Character(FightManager* manager, b2Vec2 pos, char input, ushort player, float w = 3.f, float h = 3.f);
	virtual ~Character();

	void SetSpawn(b2Vec2 spawn, short dir);
	void SetPNumber(ushort num);

	virtual void update() override;
	void UpdateAnimations();
	void AllowAttack(bool isInMove = true, bool includeTaunt = true);
	void AllowMovement(bool changeDirection = false, bool showParticles = false);
	void AllowMovement(float multiplier, bool changeDirection = false, bool showParticles = false);
	void StunBehaviour();
	void DropPlatform();
	virtual void draw() override;
	virtual void draw(SDL_Rect* camera) override;
	virtual void drawHUD(ushort numOfPlayer) ;

	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;
	void SuccessfulHit(bool shieldBreak, HitData& a, bool& controlHitLag, Entity* attacker, bool& controlShake, bool& controlCamShake);
	bool IsGoingToKill(const b2Vec2& angle);
	virtual SDL_Rect* GetHurtbox();
	Texture* getPortrait() { return portrait; };

	virtual void OnDeath() override;
	void AddDeathParticle();
	virtual void Respawn();

	virtual void ResetChar();

	virtual void BasicNeutral(ushort frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void BasicForward(ushort frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void BasicUpward(ushort frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void BasicDownward(ushort frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};

	virtual void SpecialNeutral(ushort frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void SpecialForward(ushort frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void SpecialUpward(ushort frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};
	virtual void SpecialDownward(ushort frameNumber) {
		currentMove = nullptr;
		moveFrame = -1;
	};


	virtual void StartJump(ushort frameNumber);
	virtual void StartShield(ushort frameNumber);
	virtual void EndShield(ushort frameNumber);
	virtual void Dash(ushort frameNumber);
	virtual void DashLanding(ushort frameNumber);
	virtual void Taunt(ushort frameNumber);
	virtual void Elements();

	void StartMove(std::function<void(ushort)> newMove);
	void ChangeMove(std::function<void(ushort)> newMove);

	int getInput() { return input_; }
	int getDamageTaken() { return totalDamageTaken; }
	int getDeaths() { return maxLives - lives; }
	virtual bool isCharacter() override { return true; };
};