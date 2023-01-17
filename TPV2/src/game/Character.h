#pragma once

#include "Entity.h"
#include "../sdlutils/InputHandler.h"
#include "Utils/InputConfig.h"
#include "../json/json.hpp"
#include <fstream>
#include "Utils/HUDManager.h"

using json = nlohmann::json;

class Character abstract : public Entity
{
protected:
	ushort playerNumber = 0;
	ushort playerPosition = 0;//P1 ,P2 ,P3,P4..
	InputConfig *input = nullptr;
	InputConfig* ogInput = nullptr;

	AnimationManager* anim = nullptr;

	string jsonPath;

	bool waitingToRespawn = false;

	//Datos de los ataques (Deberian salir de jsons en un futuro)
	std::unordered_map<std::string, attackData> attacks = unordered_map<string, attackData>();

	Texture* arrowsTex = nullptr;
	Texture* portrait = nullptr;
	SDL_Rect arrowSrc = { };
	ushort stun = 0;
	ushort lives = 0;

	HudManager* hud;

	const ushort maxLives = 3;

	string codeName = "";
	bool moving = false;
	bool hasRealasedUp = false;
	ushort maxSpeed = 0;
	float speed = 0.f;

	ushort shield = 0;
	ushort shieldHealth = 0;
	int maxShield = 0;
	ushort parry = 0;
	ushort parryWindow = 7;

	bool dash = false;

	//Variables para el salto (fuerza, maximo numero permitido, y el contador)
	ushort jumpStr = 0;
	ushort maxJumps = 0;
	ushort jumpCounter = 0;
	bool jumpCooldown = 0;
	ushort maxFallCount = 10; // Para iniciar los dos siguientes contadores:
	ushort fall = 0; // Contador entre pulsaciones de abajo (para bajar plataformas)
	ushort reactivateColl = 0; // Contador para reactivar colisiones con plataforma
	bool down = false; // El jugador pulsó abajo

	ushort weight = 0;

	ushort damageTaken = 0;
	ushort totalDamageTaken = 0;

	const float stunnedBounciness = 0.8f;
	
	bool recovery = true;

	//frame actual del movimiento que este haciendo
	short moveFrame = 0;

	int respawnFrames = 150;
	//Metodo del movimiento que este haciendo (esto es una variable que guarda metodos :v)
	std::function<void(int)> currentMove;

	json ReadJson(const std::string& file, spriteSheetData& spData);

	// efectos de estado
	enum state efEstado = none;
	ushort statePower = 0;
	ushort stateCont = 0;
	ushort stateDur = 300;
	float ralentizar = 0;
//	void (Character::* currentMove)(int);
	ushort input_ = 0;

	unsigned int arrowCont = 0;
	bool invencible = false;
	bool drawArrow = false;
	unsigned int invencibleCont = 0;

	std::string animAddon = "";

	Vector2D eyePos = { 0,0 };

	virtual void CreateHitBox(HitBoxData* data);

	virtual void BuildBoxes() = 0;

	virtual Vector2D BuildBoxOffset(const HitBoxData& data) {
		return Vector2D((data.box.x + ((float)data.box.w / 2.f)) - (hurtbox.x + ((float)hurtbox.w / 2.f)), (data.box.y + ((float)data.box.h / 2.f)) - (hurtbox.y + ((float)hurtbox.h / 2.f)));
	};

	virtual void BuildParticlePool() override;

	void ChangeTexMod(Uint8 r, Uint8 g, Uint8 b)
	{
		Uint8 r_, g_, b_;
		texture->GetTexMod(r_, g_, b_);
		if (r != r_ || g != g_ || b != b_)
		{
			texture->SetTexMod(r, g, b);
		}
	}

public:

	Character(FightManager* manager, b2Vec2 pos, char input, ushort player, float w = 3.f, float h = 3.f);
	virtual ~Character();

	virtual void SetSpawn(b2Vec2 spawn, short dir);
	void SetPNumber(ushort num);
	ushort GetPNumber() { return playerNumber; };

	virtual void OnFightBegin() { hud = manager->GetHUD(); };

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

	AnimationManager* GetAnimationManager() { return anim; };

	void ChangeDir(short newDir) { dir = newDir; };

	virtual void CheckHits() override;
	virtual bool GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake) override;

	virtual void OnParry(Entity* attacker, bool& controlHitLag, HitData& a, bool& controlCamShake, bool& controlShake);
	bool IsParrying() { return parry > 0 && parry <= parryWindow; }
	void SuccessfulHit(bool shieldBreak, HitData& a, bool& controlHitLag, Entity* attacker, bool& controlShake, bool& controlCamShake);
	bool IsGoingToKill(const b2Vec2& angle);
	virtual SDL_Rect* GetHurtbox();
	Texture* getPortrait() { return portrait; };

	void SetDamage(ushort a) { damageTaken = a; };

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

	virtual InputConfig* GetInputConfig() { return input; };
	virtual InputConfig* GetOgInput() { return ogInput; };
	virtual void SetInputConfig(InputConfig* value) { input = value; };
	virtual SDL_Rect getCurrentSpriteSrc() override;
	int getInput() { return input_; }
	ushort GetDamageTaken() { return damageTaken; }
	ushort getDamageTaken() { return totalDamageTaken; }
	int getDeaths() { return maxLives - lives; }
	void setRecovery(bool dime) { recovery = dime; };
	void SetLives(ushort newLives) { lives = newLives; };
	ushort GetLives() { return lives; };
};