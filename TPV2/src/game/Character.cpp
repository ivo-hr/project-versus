#include "Character.h"
#include "Utils/AnimationManager.h"
#include "Utils/InputConfig.h"
#include "Utils/Particle.h"
#include "Characters/NasNas/Explosion.h"
#include "../utils/CheckML.h"
#include <iostream>


json Character::ReadJson(std::string filename, spriteSheetData &spData)
{
	std::ifstream file(filename);
	json jsonFile;
	file >> jsonFile;


	// variables
	codeName = jsonFile["codeName"];
	weight = jsonFile["weight"];
	maxSpeed = jsonFile["maxSpeed"];
	speed = jsonFile["speed"];
	maxJumps = jsonFile["maxJumps"];
	jumpStr = jsonFile["jumpStr"];
	maxShield = jsonFile["maxShield"];
	jumpCounter = maxJumps;
	damageTaken = 0;
	shieldHealth = maxShield;
	jumpCooldown = true;


	//Aqui defino las caracteristicas de cada hitbox (podriamos hacerlo dentro de cada metodo, y vendria de json)(tambien podríamos poner framedata)


	attackData aux;
	auto aData = jsonFile["attacksData"];
	assert(aData.is_array());

	for (uint16 i = 0u; i < aData.size(); i++) {

		HitBoxData hitboxDataAux;

		auto hitBoxes = aData[i]["hitBoxes"];
		assert(hitBoxes.is_array());

		for (uint16 j = 0u; j < hitBoxes.size(); j++)
		{
			hitboxDataAux.hitdata.damage = hitBoxes[j]["hitData"]["damage"];
			hitboxDataAux.hitdata.direction = b2Vec2(hitBoxes[j]["hitData"]["b2vecX"], hitBoxes[j]["hitData"]["b2vecY"]);
			hitboxDataAux.hitdata.direction.Normalize();
			hitboxDataAux.hitdata.base = hitBoxes[j]["hitData"]["base"];
			hitboxDataAux.hitdata.multiplier = hitBoxes[j]["hitData"]["multiplier"];
			hitboxDataAux.hitdata.stun = hitBoxes[j]["hitData"]["stun"];
			hitboxDataAux.hitdata.shieldBreak = hitBoxes[j]["hitData"]["shieldBreak"];

			hitboxDataAux.hitlag = hitBoxes[j]["hitlag"];
			hitboxDataAux.duration = hitBoxes[j]["duration"];

			aux.hitBoxes.push_back(hitboxDataAux);
		}

		assert(aData[i]["keyFrames"].is_array());
		for (uint16 j = 0u; j < aData[i]["keyFrames"].size(); j++)
			aux.keyFrames.push_back(aData[i]["keyFrames"][j]);

		aux.totalFrames = aData[i]["totalFrames"];

		attacks.insert({ aData[i]["id"], aux });

		aux.keyFrames.clear();
		aux.hitBoxes.clear();
	}


	//Datos para las animaciones (tendrá que venir de json claramente solo hay tres y ya ocupan 37 lineas xd)
	auto sData = jsonFile["spData"];
	//Mirando a la derecha

	spData.leftOffset = sData["leftOffset"];		//Pixeles en sprite que se dibujaran fuera de la hurtbox a la izquierda
	spData.upOffset = sData["upOffset"];
	spData.sizeXOffset = sData["sizeXOffset"];	//Cuantos pixeles en X NO estan dentro de la hurtbox
	spData.sizeYOffset = sData["sizeYOffset"];

	spData.spritesInX = sData["spritesInX"];
	spData.spritesInY = sData["spritesInY"];

	animationData auxAnim;
	auto animData = jsonFile["animationData"]["anim"];
	assert(animData.is_array());

	for (uint16 i = 0u; i < animData.size(); i++) {

		auxAnim.iniSprite = animData[i]["iniSprite"];
		auxAnim.totalSprites = animData[i]["totalSprites"];

		std::vector<ushort> a = animData[i]["keySprite"];
		auxAnim.keySprite = a;

		if (animData[i]["attack"] != "")
		{
			auxAnim.keyFrame = attacks[animData[i]["attack"]].keyFrames;
			auxAnim.totalFrames = attacks[animData[i]["attack"]].totalFrames;
		}
		else
		{
			//auxAnim.keySprite = -1;
			auxAnim.totalFrames = animData[i]["totalFrames"];
		}
		auxAnim.loop = animData[i]["loop"];

		spData.animations.insert({ animData[i]["id"], auxAnim });
		auxAnim.keyFrame.clear();
	}

	BuildBoxes();

	for (auto& a : attacks)
	{
		for (HitBoxData& hitboxes : a.second.hitBoxes)
		{
			hitboxes.normalOffset = BuildBoxOffset(hitboxes);
		}
	}

	return jsonFile;
}

void Character::CreateHitBox(HitBoxData* data)
{
	data->charOffset = Vector2D(data->normalOffset.getX() * dir, data->normalOffset.getY());
	hitboxes.push_back(data);
}

void Character::BuildParticlePool()
{
	particlePool["smallHit"].push_front(new Particle({ 0,0 }, "sHitParticle", this, 1));
	particlePool["bigHit"].push_front(new Particle({ 0,0 }, "bHitParticle", this, 1));
	particlePool["run"].push_front(new Particle({ 0,0 }, "run", this, 1));
	particlePool["water"].push_front(new Particle({ 0,0 }, "water", this, 1));
	particlePool["electric"].push_front(new Particle({ 0,0 }, "electric", this, 1));
	particlePool["fire"].push_front(new Particle({ 0,0 }, "fire", this, 1));

	particlePool["died"].push_front(new Particle({ 0,0 }, "died", this, 1));
	particlePool["killHit"].push_front(new Particle({ 0,0 }, "killHit", this, 1));
	particlePool["killVfx"].push_front(new Particle({ 0,0 }, "killVfx", this, 1));
	particlePool["shieldBroken"].push_front(new Particle({ 0,0 }, "shieldBroken", this, 1));
	particlePool["parryS"].push_front(new Particle(
		{ 0,0 }, ParticleData(&sdl->images().at("parryS"), SDL_Rect({ 0, 0, 48, 48 }), 4, 2, 2, 16), this)
	);
	particlePool["parryB"].push_front(new Particle(
		{ 0,0 }, ParticleData(&sdl->images().at("parryB"), SDL_Rect({ 0, 0, 128, 64 }), 5, 3, 2, 25), this)
	);
}

Character::Character(FightManager* manager, b2Vec2 pos, char input, ushort playerPos, float w, float h) :
	Entity(manager, pos, w, h)
{
	arrowsTex = &sdl->images().at("arrows");
	playerPosition = playerPos;
	arrowSrc.x = 0;
	arrowSrc.y = 0;
	arrowSrc.w = arrowsTex->width();
	arrowSrc.h = arrowsTex->height() / 4;

	stun = 0;
	dash = false;
	shield = 0;
	lives = maxLives;
	this->input = new InputConfig(input);
	input_ = input;
	totalDamageTaken = 0;
	kills = 0;
	resetLastCharacter();
	BuildParticlePool();
}

Character::~Character()
{
	r = 0;
	g = 255;
	delete input;
	delete anim;
}

void Character::SetSpawn(b2Vec2 spawn, short dir)
{
	body->SetTransform(spawn, 0);
	this->dir = dir;
	respawnPos = b2Vec2(spawn.x, 10);
}

void Character::SetPNumber(ushort num)
{
	playerNumber = num;
	arrowSrc.y = arrowSrc.h * num;
}

void Character::update()
{
	// Para probar mandos
	//input->controllerTest();
	if (invencible && invencibleCont + 3000 < SDL_GetTicks()) {
		invencible = false;
		dash = false;
	}

	updateParticles();

	if (hitLag % 3 == 1)
		shakeValue = { shakeValue.getX() * -0.9f, shakeValue.getY() * -0.9f };

	if (hitLag > 0)
	{
		hitLag--;
		if (hitLag == 0)
		{
			body->SetEnabled(true);
			parry = parryWindow + 1;
			shakeValue = { 0, 0 };
		}
		return;
	}

	if (!alive)
	{
		respawnFrames--;
		if (respawnFrames == 0)
		{
			Respawn();
			respawnFrames = 150;
		}
		return;
	}

	if (stun > 0)
	{
		stun--;
		if (!recovery) recovery = true;
	}

	//recuperar escudo

	DropPlatform();

	if (stun > 0)
	{
		StunBehaviour();
	}
	else
	{ 
		if (currentMove == nullptr)
		{
			//----------------Movimiento
			if (onGround)
			{
				AllowMovement(true, true);
			}
			else
			{
				AllowMovement(0.7f, true, true);
			}

			if (recovery)
			{
				//----------------Salto
				if (input->up() && !(jumpCounter <= 0 || !jumpCooldown))
				{
					StartMove([this](int f) { StartJump(f); });
				}

				//----------------Atacar
				AllowAttack(false);

				if (onGround)
				{
					//Escudo
					if (input->down() && shieldHealth > (maxShield / 8) && (body->GetLinearVelocity().y > -0.1f && body->GetLinearVelocity().y < 0.1f)) {

						StartMove([this](int f) { StartShield(f); });
						body->SetLinearVelocity(b2Vec2(0, 0));

					}
				}
				else
				{
					if (input->down())
					{
						StartMove([this](int f) { Dash(f); });
					}
				}

				UpdateAnimations();
			}
		}
	}
	if (!recovery) {
		if (anim->CurrentAnimation() != "dash" + animAddon)
			anim->StartAnimation("dash" + animAddon);

	}

	//Si hay un movimiento en ejecucion lo continuamos...
	if (currentMove != nullptr)
	{
		//ejecuta el ataque guardado en la variable
		(currentMove)(moveFrame);
		//Actualiza el frame actual del movimiento
		moveFrame++;
	}

	//para recuperar escudo
	if (!shield && shieldHealth < maxShield)
	{
		shieldHealth++;
	}

	//Chequeo de tierra
	if (GetGround())
	{
		jumpCounter = maxJumps;
		if (!recovery) recovery = true;
	}

	//chequeo doble salto
	if (!input->up() && !jumpCooldown)
	{
		jumpCooldown = true;
	}

	if (stun == 0)
		body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));

	if (speed > maxSpeed * 0.2f)
		speed -= maxSpeed * 0.2f;
	else if (speed < -maxSpeed * 0.2f)
		speed += maxSpeed * 0.2f;
	else
		speed = 0;


	// Efectos de estado
	if (efEstado != none && stateCont < stateDur)
	{
		stateCont++;
		if (stateCont % 60 == 0)
		{
			if ((efEstado == fire || efEstado == wElectric))
			{
				Elements();
			}

			if (efEstado == fire)
			{
				AddParticle("fire", Vector2D(hurtbox.x, hurtbox.y), 1, true);
			}
			else if (efEstado == water)
			{
				AddParticle("water", Vector2D(hurtbox.x, hurtbox.y), 1, true);
			}
			else if (efEstado == wElectric)
			{
				AddParticle("electric", Vector2D(hurtbox.x, hurtbox.y), 1, true);
			}
		}
	}
	else
	{
		if (efEstado == water)
		{
			maxSpeed += ralentizar;
			ralentizar = 0;
		}
		stateCont = 0;
		statePower = 0;
		efEstado = none;
	}

	hurtbox.x = manager->b2ToSDLX(body, width);
	hurtbox.y = manager->b2ToSDLY(body, height);

	anim->update();

	if (!SDL_HasIntersection(&hurtbox, manager->GetDeathZone()) && (hurtbox.y > -hurtbox.h || !invencible))
	{
		OnDeath();
	}

}

void Character::UpdateAnimations()
{

	if (!GetGround())
	{
		if (body->GetLinearVelocity().y > 0.01f && anim->CurrentAnimation() != "airborne" + animAddon)
			anim->StartAnimation("airborne" + animAddon);
		else if (body->GetLinearVelocity().y < -0.01f && anim->CurrentAnimation() != "jump" + animAddon)
			anim->StartAnimation("jump" + animAddon);
	}
	else
	{
		if (speed > 0.1f || speed < -0.1f)
		{
			if (anim->CurrentAnimation() != "run" + animAddon)
				anim->StartAnimation("run" + animAddon);
			sdl->soundEffects().at(codeName + "Steps").play();

		}
		//frenarse
		else
		{
			if (anim->CurrentAnimation() != "idle" + animAddon)
				anim->StartAnimation("idle" + animAddon);
		}
	}
}

void Character::AllowAttack(bool isInMove, bool includeTaunt)
{
	// Ataque con A (provisional)

	if (input->basic())
	{
		sdl->soundEffects().at(codeName + "Steps").haltChannel();

		if (input->up()) //básico arriba
		{
			if (isInMove)
				ChangeMove([this](int f) { BasicUpward(f); });
			else
				StartMove([this](int f) { BasicUpward(f); });
		}
		else if (input->down()) //básico abajo
		{
			if (isInMove)
				ChangeMove([this](int f) { BasicDownward(f); });
			else
				StartMove([this](int f) { BasicDownward(f); });
		}
		else if (input->right() || input->left()) //básico en movimiento
		{
			if (isInMove)
				ChangeMove([this](int f) { BasicForward(f); });
			else
				StartMove([this](int f) { BasicForward(f); });
		}
		else //básico estático
		{
			if (isInMove)
				ChangeMove([this](int f) { BasicNeutral(f); });
			else
				StartMove([this](int f) { BasicNeutral(f); });
		}

		manager->MoveToFront(this);

	}

	// Ataque con B (provisional)
	if (input->special())
	{
		sdl->soundEffects().at(codeName + "Steps").haltChannel();


		if (input->up()) //especial arriba
		{
			if (isInMove)
				ChangeMove([this](int f) { SpecialUpward(f); });
			else
				StartMove([this](int f) { SpecialUpward(f); });
		}
		else if (input->down()) //especial abajo
		{
			if (isInMove)
				ChangeMove([this](int f) { SpecialDownward(f); });
			else
				StartMove([this](int f) { SpecialDownward(f); });
		}
		else if (input->right() || input->left()) //especial en movimiento
		{
			if (isInMove)
				ChangeMove([this](int f) { SpecialForward(f); });
			else
				StartMove([this](int f) { SpecialForward(f); });
		}
		else //especial estático
		{
			if (isInMove)
				ChangeMove([this](int f) { SpecialNeutral(f); });
			else
				StartMove([this](int f) { SpecialNeutral(f); });
		}

		manager->MoveToFront(this);

	}

	if (includeTaunt)
	{
		if (input->taunt() && onGround)
		{
			if (isInMove)
				ChangeMove([this](int f) { Taunt(f); });
			else
				StartMove([this](int f) { Taunt(f); });

			manager->MoveToFront(this);
		}
	}
}

void Character::AllowMovement(bool changeDirection, bool showParticles)
{

	if (input->right() && input->left())
	{
		speed = 0;
	}
	else
	{

		if (input->right())
		{
			if (changeDirection)
				dir = 1;

			if (showParticles && speed < 0.01f)
				AddParticle("run", Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, false);

			speed = maxSpeed;
		}
		if (input->left())
		{
			if (changeDirection)
				dir = -1;

			if (showParticles && speed > -0.01f)
				AddParticle("run", Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, false);

			speed = -maxSpeed;
		}
	}
}

void Character::AllowMovement(float multiplier, bool changeDirection, bool showParticles)
{

	if (input->right() && input->left())
	{
		speed = 0;
	}
	else
	{

		if (input->right())
		{
			if (changeDirection)
				dir = 1;

			if (showParticles && speed < 0.01f)
				AddParticle("run", Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, false);

			speed = maxSpeed * multiplier;
		}
		if (input->left())
		{
			if (changeDirection)
				dir = -1;

			if (showParticles && speed > -0.01f)
				AddParticle("run", Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, false);

			speed = -maxSpeed * multiplier;
		}
	}
}

void Character::StunBehaviour()
{

	if (anim->CurrentAnimation() != "stun" + animAddon)
		anim->StartAnimation("stun" + animAddon);

	if (!onGround)
	{
		if (input->right())
		{
			body->ApplyLinearImpulseToCenter({ 4, 0 }, true);
		}
		if (input->left())
		{
			body->ApplyLinearImpulseToCenter({ -4, 0 }, true);
		}
	}
	else
	{
		speed = 0;
	}

	currentMove = nullptr;
}

void Character::DropPlatform()
{

	// bajar plataformas
	if (down && input->downReleased()) {
		down = false;
		fall = maxFallCount; // Activa contador para reconocer el bajar plataformas
	}
	if (input->down() && body->GetFixtureList()->GetFilterData().maskBits != 2) down = true; // Marca que se ha pulsado abajo (para el tema de bajar plataformas)

	if (onGround && fall > 0) {
		fall--;
		if (input->down()) { // Va a atravesar la plataforma
			reactivateColl = maxFallCount;
			for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
				auto fix = f->GetFilterData();
				fix.maskBits = 2; // Quita la colisión con la plataforma momentáneamente
				f->SetFilterData(fix);
			}
			fall = 0;
		}
	}

	if (reactivateColl > 0) reactivateColl--;
	if (reactivateColl == 0 && body->GetFixtureList()->GetFilterData().maskBits == 2) { // Tras medio segundo reactiva colisión jugador-plataformas
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			auto fix = f->GetFilterData();
			fix.maskBits = 2 | 4;
			f->SetFilterData(fix);
		}
	}
}

void Character::draw()
{
	//xd
	
		Entity::draw();

		if (!alive) return;

		anim->render();
	

	//if (debug)
#ifdef _DEBUG

	if (parry > 0 && parry <= parryWindow)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 255, 255);
	}
	else if (shield)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 0, 255, 255);
	}
	else if (dash)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 255, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 255, 0, 255);
	}
	SDL_RenderDrawRect(sdl->renderer(), &hurtbox);

#endif // _DEBUG

}

void Character::draw(SDL_Rect* camera)
{
	//xd
	for (Particle* ent : backParticles)
	{
		ent->draw(camera);
	}

	if (drawArrow && alive)
	{
		Entity::draw(camera);
		anim->render(camera, &shakeValue);
	}

	for (Particle* ent : frontParticles)
	{
		ent->draw(camera);
	}

	SDL_Rect aux = hurtbox;

	aux.x -= camera->x;
	aux.x = int((float)aux.x * ((float)manager->GetActualWidth() / (float)camera->w));

	aux.y -= camera->y;
	aux.y = int((float)aux.y * (float)manager->GetActualWidth() / (float)camera->w);

	aux.w = int((float)aux.w * (float)manager->GetActualWidth() / (float)camera->w);
	aux.h = int((float)aux.h * (float)manager->GetActualWidth() / (float)camera->w);

	if (invencible)
	{
		if (arrowCont + 100 < SDL_GetTicks()) {
			arrowCont = SDL_GetTicks();
			drawArrow = !drawArrow;
		}		
	}
	else
	{
		drawArrow = true;
	}

	int xpos = aux.x + (aux.w / 2);

	arrowsTex->render(arrowSrc, { xpos - 15, aux.y - 44, 30, 16 });

#ifdef _DEBUG

	if (parry > 0 && parry <= parryWindow)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 255, 255);
	}
	else if (shield)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 0, 255, 255);
	}
	else if (dash)
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 255, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 255, 0, 255);
	}


	SDL_RenderDrawRect(sdl->renderer(), &aux);

#endif // _DEBUG
}


void Character::CheckHits()
{
	bool toResetHits = false;

	if (hitLag > 0)
	{
		return;
	}

	for (int i = 0; i < hitboxes.size(); i++)
	{

		if (hitboxes[i]->duration == hitboxes[i]->outFor)
		{
			hitboxes[i]->outFor = 0;
			for (int j = i + 1; j < hitboxes.size(); j++)
			{
				hitboxes[(size_t)j - 1] = hitboxes[j];
			}
			hitboxes.pop_back();
			i--;
			toResetHits = true;
		}
		else
		{
			hitboxes[i]->box.x = (hurtbox.x + (hurtbox.w / 2) + hitboxes[i]->charOffset.getX()) - hitboxes[i]->box.w / 2;
			hitboxes[i]->box.y = (hurtbox.y + (hurtbox.h / 2) + hitboxes[i]->charOffset.getY()) - hitboxes[i]->box.h / 2;
			hitboxes[i]->outFor++;
		}

		if (!toResetHits)
		{
			Entity* oponent = nullptr;
			while (manager->GetNextEntity(oponent, layer))
			{
				SDL_Rect hitArea;
				if (SDL_IntersectRect(&hitboxes[i]->box, oponent->GetHurtbox(), &hitArea) && !isHit[oponent])
				{
					manager->MoveToFront(this);
					bool hitLagApplied = false, shakeApplied = false, camShakeApplied = false;
					//Le hace daño xddd
					if (oponent->GetHit(hitboxes[i]->hitdata, this, hitLagApplied, shakeApplied, camShakeApplied))
					{
						if (!hitLagApplied)
						{
							AddHitLag(hitboxes[i]->GetHitlag());
							oponent->AddHitLag(hitboxes[i]->GetHitlag());
						}

						if (!shakeApplied)
						{
							oponent->SetShake(Vector2D(hitboxes[i]->hitdata.direction.x, hitboxes[i]->hitdata.direction.y), hitboxes[i]->GetHitlag());
						}

						if (!camShakeApplied)
						{
							manager->SetShake(Vector2D(hitboxes[i]->hitdata.direction.x * -hitboxes[i]->GetHitlag() * 0.3f, hitboxes[i]->hitdata.direction.y * hitboxes[i]->GetHitlag() * 0.1f), hitboxes[i]->GetHitlag());
						}

						oponent->setLastCharacer(this);

						if (hitboxes[i]->GetHitlag() >= 15)
						{
							AddParticle("bigHit", 
								Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
								1, false);

							manager->GetSDLU()->soundEffects().at("hitStr").play();
						}
						else
						{
							AddParticle("smallHit",
								Vector2D(hitArea.x + hitArea.w / 2, hitArea.y + hitArea.h / 2),
								1, false);

							manager->GetSDLU()->soundEffects().at("hitMed").play();
						}
					}
					isHit[oponent] = true;
				}
			}
		}
	}
	if (toResetHits)
	{
		resetHit();
	}
}

bool Character::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake)
{
	//Parry
	if (parry > 0 && parry <= parryWindow)
	{
		if (!attacker->isProjectile())
		{
			AddHitLag(20);
			attacker->AddHitLag(40);
		}
		else
		{
			AddHitLag(4);
		}
		currentMove = nullptr;
		moveFrame = 0;
		parry = 1;
		shield = 0;
		anim->StartAnimation("parry" + animAddon);
		anim->update();
		manager->MoveToFront(this);
		controlHitLag = true;

		manager->SetShake(Vector2D(a.direction.x * 2, a.direction.y * 3), 3);
		controlCamShake = true;

		float xEyeDiff = eyePos.getX() - (hurtbox.w / 2.f);

		AddParticle("parryS", { hurtbox.x + (hurtbox.w / 2.f) + (xEyeDiff * dir), hurtbox.y + eyePos.getY() }, dir, true);
		AddParticle("parryB", { hurtbox.x + hurtbox.w / 2.f, (float)hurtbox.y + hurtbox.h * 1.05f });

		sdl->soundEffects().at("parry").play();

		return false;
	}
	//Shield is up
	if (shield > 0)
	{

		//See if it broke shield
		if (a.damage > shieldHealth || a.shieldBreak)
			shieldHealth = 0;
		else
			shieldHealth -= a.damage;

		//Shield NOT broken
		if (shieldHealth > 0)	//A
		{
			damageTaken += (ushort)(a.damage * 0.4f);
			controlHitLag = false;
		}
		//Shield broken
		else
		{
			SuccessfulHit(true, a, controlHitLag, attacker, controlShake, controlCamShake);
		}
		return true;
	}
	//Is dodging
	if (dash)
	{
		return false;
	}
	//Normal hit
	else if (!shield && !dash && !attacker->ToDelete())
	{
		SuccessfulHit(false, a, controlHitLag, attacker, controlShake, controlCamShake);
		return true;
	}

}

void Character::SuccessfulHit(bool shieldBreak, HitData& a, bool& controlHitLag, Entity* attacker, bool& controlShake, bool& controlCamShake)
{
	hitboxes.clear();
	resetHit();

	body->SetGravityScale(10.0f);
	currentMove = nullptr;
	moveFrame = -1;
	anim->StartAnimation("stun" + animAddon);
	anim->update();

	float recoil;
	if (shieldBreak)
		recoil = (a.base * 1.5f + ((damageTaken * a.multiplier) / (weight * .1f)));
	else
		recoil = (a.base + ((damageTaken * a.multiplier) / (weight * .2f)));

	stun = a.GetStun(recoil);

	controlHitLag = false;

	//Actualiza el da�o
	if (shieldBreak)
		damageTaken += a.damage * 2;
	else
		damageTaken += a.damage;

	b2Vec2 aux = a.direction;

	aux *= recoil;
	aux.y *= -1;
	aux.x *= attacker->GetDir();

	if (onGround && aux.y > 0)
	{
		aux.y *= -0.9f;
	}

	if (IsGoingToKill(aux))
	{
		ushort currHitlag;
		if (shieldBreak)
			currHitlag = 50;
		else
			currHitlag = 40;

		manager->KillingBlow();

		AddHitLag(currHitlag);
		attacker->AddHitLag(currHitlag);
		controlHitLag = true;

		SetShake(Vector2D(a.direction.x, a.direction.y), currHitlag / 3);
		controlShake = true;

		manager->SetShake(Vector2D(a.direction.x * -15, a.direction.y * 10), currHitlag / 2);
		controlCamShake = true;

		AddParticle("killVfx", 
			Vector2D(
				manager->ToSDL(body->GetPosition().x),
				manager->ToSDL(body->GetPosition().y)),
			1, false);
		AddParticle("killHit", 
			Vector2D(
				manager->ToSDL(body->GetPosition().x),
				manager->ToSDL(body->GetPosition().y)),
			1, false);
	}
	else if (shieldBreak)
	{
		sdl->soundEffects().at("shieldBreak").play();

		AddHitLag(30);
		attacker->AddHitLag(30);
		controlHitLag = true;

		SetShake(Vector2D(a.direction.x, a.direction.y), 30);
		controlShake = true;

		manager->SetShake(Vector2D(a.direction.x * -15, a.direction.y * 7), 30);
		controlCamShake = true;
	}

	if (shieldBreak)
	{
		shield = 0;
		shieldHealth = 0;
		controlHitLag = true;

		AddParticle("shieldBroken", 
			Vector2D(
				manager->ToSDL(body->GetPosition().x),
				manager->ToSDL(body->GetPosition().y)),
			1, true);
	}

	// Estados y combinaciones de estado
	if (a.estado != none)
	{
		if (efEstado != a.estado && efEstado != none)
		{
			// fuego y rayo
			if ((efEstado == fire && a.estado == electric) || (efEstado == electric && a.estado == fire))
			{
				//explosión de fuego/rayo
				// 
				//suma de efectos de estado para pasarselo a la explosion
				int poder = statePower + a.power;
				auto plasma = new Explosion(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - height / 2), poder, 1, attacker->GetDir() == 1);
				manager->AddEntity(plasma, 0);
				manager->MoveToFront(plasma);
				efEstado = none;
				statePower = 0;
				stateCont = 0;
			}
			//fuego y agua
			else if ((efEstado == fire && a.estado == water) || (efEstado == water && a.estado == fire))
			{
				if (efEstado == water)
				{
					maxSpeed += ralentizar;
					ralentizar = 0;
				}
				int poder = (statePower + a.power) / 3;
				auto vapor = new Explosion(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - height / 2), poder, 0, attacker->GetDir() == 1);
				manager->AddEntity(vapor, 0);
				manager->MoveToFront(vapor);
				efEstado = none;
				statePower = 0;
				stateCont = 0;
			}
			// agua y rayo
			else if ((efEstado == water && a.estado == electric) || (efEstado == electric && a.estado == water))
			{
				if (efEstado == water)
				{
					maxSpeed += ralentizar;
					ralentizar = 0;
				}
				if (a.estado == electric)
				{
					stun += statePower * 1.5f;

					AddParticle("electric", Vector2D(hurtbox.x, hurtbox.y), 1, true);
				}
				efEstado = wElectric;
				statePower += a.power;
				stateCont = 0;

			}
		}
		//si se recibe un ataque con un estado del cual ya se veía afectado
		else if (efEstado == a.estado)
		{
			stateCont = 0;
			statePower = a.power;
			if (efEstado == electric)
			{
				stun += statePower * 1.5f;
			}
		}
		//efecto de estado básico
		else if (efEstado == none)
		{
			efEstado = a.estado;
			statePower = a.power;
			if (efEstado == electric)
			{
				stun += statePower * 1.5f;

				AddParticle("electric", Vector2D(hurtbox.x, hurtbox.y), 1, true);
			}
			else if (efEstado == water)
			{
				ralentizar = maxSpeed * ((float)statePower / 100);
				maxSpeed -= ralentizar;

				AddParticle("water", Vector2D(hurtbox.x, hurtbox.y), 1, true);
			}
			else if (efEstado == fire)
			{
				AddParticle("fire", Vector2D(hurtbox.x, hurtbox.y), 1, true);
			}
		}
	}

	//Produce el knoback..
	body->SetLinearVelocity(aux);
}

bool Character::IsGoingToKill(const b2Vec2& angle)
{
	float horValue = 0.02f;  //0.8f
	float upValue = 0.0037f;
	b2Vec2 pos = body->GetPosition();

	return (pos.x + (angle.x * abs(angle.x) * horValue) > manager->GetDeathZoneB2()->x ||
		pos.x + (angle.x * abs(angle.x) * horValue) < 0 ||
		pos.y + (angle.y * abs(angle.y) * upValue) < 0);
}


void Character::StartJump(ushort frameNumber)
{
	AllowMovement();

	if (jumpCounter <= 0 || !jumpCooldown)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (frameNumber == 0)
	{
		hasRealasedUp = false;
		anim->StartAnimation("jumpCharge" + animAddon);
	}
	if (frameNumber <= 5)
	{
		if (!input->up())
		{
			hasRealasedUp = true;
		}
	}
	if (frameNumber <= 2)
	{
		if (input->special())
		{
			ChangeMove([this](int f) { SpecialUpward(f); });
			moveFrame = -1;
		}
		else if (input->basic())
		{
			ChangeMove([this](int f) { BasicUpward(f); });
			moveFrame = -1;
		}
	}
	else if (frameNumber >= 4)
	{
		sdl->soundEffects().at(codeName + "Steps").haltChannel();

		anim->StartAnimation("jump" + animAddon);
		if (!GetGround())
		{
			sdl->soundEffects().at("jump1").play();

			jumpCounter--;
		}
		else sdl->soundEffects().at("jump0").play();

		jumpCooldown = false;
		body->SetLinearVelocity(b2Vec2(speed, 0)); 

		if (input->basic())
		{

			if (input->up()) //básico arriba
			{
				ChangeMove([this](int f) { BasicUpward(f); });
			}
			else if (input->right() || input->left()) //básico en movimiento
			{
				ChangeMove([this](int f) { BasicForward(f); });
			}
			else //básico estático
			{
				ChangeMove([this](int f) { BasicNeutral(f); });
			}

			manager->MoveToFront(this);

			if (!onGround)
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
			else
			{
				if (hasRealasedUp)
					body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);
				else
					body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
			}

		}
		else if (input->special())
		{

			if (input->up()) //especial arriba
			{
				ChangeMove([this](int f) { SpecialUpward(f); });
			}
			else if (input->right() || input->left()) //especial en movimiento
			{
				ChangeMove([this](int f) { SpecialForward(f); });
			}
			else //especial estático
			{
				ChangeMove([this](int f) { SpecialNeutral(f); });
			}

			manager->MoveToFront(this);


			if (!onGround)
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
			else
			{
				if (hasRealasedUp)
					body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);
				else
					body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
			}

		}
		else
		{
			if (input->up() || !onGround)
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
			else
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);
		}

		onGround = false;
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Character::StartShield(ushort frameNumber)
{
	int shieldStartUp = 2;

	shieldHealth--;

	if (frameNumber == 0)
	{
		parry = 0;
	}

	parry++;

	if (frameNumber >= shieldStartUp)
	{
		shield++;
	}
	if (frameNumber == shieldStartUp)
	{
		sdl->soundEffects().at("shield").play();

		anim->StartAnimation("shield" + animAddon);
	}
	if (!input->down() || shieldHealth <= 0 )
	{
		ChangeMove([this](int f) { EndShield(f); });
	}
	if (input->basic())
	{
		shield = 0;
		parry = 0;
		ChangeMove([this](int f) { BasicDownward(f); });
	}
	else if (input->special())
	{
		shield = 0;
		parry = 0;
		ChangeMove([this](int f) { SpecialDownward(f); });
	}
}
void Character::EndShield(ushort frameNumber)
{
	if (frameNumber == 0)
	{
		shield = 0;
		parry = 0;
		anim->StartAnimation("idle" + animAddon);
	}
	if (frameNumber >= 5)					//Shield end lag
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}
void Character::Dash(ushort frameNumber)
{

	if (frameNumber > 0)
	{
		if (frameNumber < 6)
			body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 10));
		if (frameNumber < 60)
			AllowMovement(0.5f);
	}
	switch (frameNumber)
	{
	case 0:
		anim->StartAnimation("dash" + animAddon);
		break;
	case 6:
		dash = true;
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 30));
		break;
	case 60:
		dash = false;
		currentMove = nullptr;
		anim->StartAnimation("idle" + animAddon);
		break;
	}
	if (onGround)
	{
		ChangeMove([this](int f) { DashLanding(f); });
		dash = false;
	}
}

void Character::DashLanding(ushort frameNumber)
{
	if (frameNumber >= 10)
	{
		currentMove = nullptr;
		moveFrame = -1;
		anim->StartAnimation("idle" + animAddon);
	}
}

void Character::StartMove(std::function<void(ushort)> newMove)
{
	currentMove = newMove;
	moveFrame = 0;
}
void Character::ChangeMove(std::function<void(ushort)> newMove)
{
	currentMove = newMove;
	moveFrame = -1;
}

SDL_Rect* Character::GetHurtbox()
{
	return &hurtbox;
}

void Character::OnDeath()
{
	body->SetGravityScale(10.0f);
	//Canal 1 , (antes a veces no se escucha)
	sdl->soundEffects().at("death").play(0,1);

	alive = false;
	lives--;
	totalDamageTaken += damageTaken;

	recovery = true;
	AddDeathParticle();
	body->SetTransform(respawnPos, 0);
	body->SetAwake(false);
	currentMove = nullptr;
	moveFrame = 0;
	moving = false;
	shield = 0;
	dash = true;
	stun = 0;
	if (efEstado != none)
	{
		if(efEstado == water)
		{
			maxSpeed += ralentizar;
			ralentizar = 0;
		}
		efEstado = none;
		statePower = 0;
	}
	if (lastCharacter != nullptr) {
		lastCharacter->increaseKills();
	}

	if (lives <= 0) {
		toDelete = true;
	}

}

void Character::AddDeathParticle()
{
	//O dios mio que he creado
	AddParticle("died",
		Vector2D(
			( manager->ToSDL(body->GetPosition().x) < 0 ?
				0 : manager->ToSDL(body->GetPosition().x )) > manager->GetDeathZone()->w?
				manager->GetDeathZone()->w : manager->ToSDL(body->GetPosition().x),

			( manager->ToSDL(body->GetPosition().y) < 0 ? 
				0 : manager->ToSDL(body->GetPosition().y )) > manager->GetDeathZone()->h ?
				manager->GetDeathZone()->h : manager->ToSDL(body->GetPosition().y)),
		1, true);
}

void Character::Respawn()
{
	body->SetAwake(true);

	std::cout << "Vidas restantes: " << lives << "\n";

	body->SetLinearVelocity({ 0, 0 }); // resetea la velocidad
	speed = 0;

	alive = true;
	damageTaken = 0;
	moving = false;

	currentMove = nullptr;
	moveFrame = 0;

	resetLastCharacter();

	anim->StartAnimation("idle" + animAddon);
	r = 0;
	g = 255;

	invencible = true;
	dash = true;
	arrowCont = 0;
	invencibleCont = SDL_GetTicks();
}

void Character::ResetChar()
{
	dash = false;
	shield = 0;
	body->SetGravityScale(10.0f);
	currentMove = nullptr;
	moveFrame = -1;
}

void Character::Taunt(ushort frameNumber)
{
	//----------------Movimiento
	if (onGround)
	{
		AllowMovement(true, true);
	}
	else
	{
		AllowMovement(0.7f, true, true);
	}

	//----------------Salto
	if (input->up() && !(jumpCounter <= 0 || !jumpCooldown))
	{
		ChangeMove([this](int f) { StartJump(f); });
	}

	//----------------Atacar
	AllowAttack(true, false);

	if (onGround)
	{
		//Escudo
		if (input->down() && shieldHealth > (maxShield / 8) && (body->GetLinearVelocity().y > -0.1f && body->GetLinearVelocity().y < 0.1f)) {

			ChangeMove([this](int f) { StartShield(f); });
			body->SetLinearVelocity(b2Vec2(0, 0));

		}
	}
	else
	{
		if (input->down())
		{
			ChangeMove([this](int f) { Dash(f); });
		}
	}

	//UpdateAnimations();
	if (input->left() || input->right())
	{
		currentMove = nullptr;
		moveFrame = -1;
		return;
	}

	if (frameNumber == 0)
	{
		anim->StartAnimation("taunt" + animAddon);
		sdl->soundEffects().at(codeName + "Steps").haltChannel();
		sdl->soundEffects().at(codeName + "Taunt").play();
	}
	else if (frameNumber >= anim->GetAnimationDuration())
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}
void Character::Elements()
{
	if (efEstado == fire)
	{
		damageTaken += statePower/(stateDur/60);
	}
	else if (efEstado == wElectric)
	{
		stun += ushort((statePower / (stateDur / 60)) * 1.5f);
		body->SetLinearVelocity({ 0, 0 }); 
		ResetChar();
	}
}
void Character::drawHUD(ushort numOfPlayer)
{
	//Portrait y posiciones
	int w_ = manager->GetActualWidth();
	int h_ = manager->GetActualHeight();
	int dist = w_/ numOfPlayer;
	int offset = (w_ / 2) / numOfPlayer - w_/30;
	int x = (int)(playerPosition * dist + offset);
	int y = h_ - (h_ / 6);
	portrait->render({ x, y, w_ / 15, w_ / 15 });
	//Porcentaje
	string fontstring = "nes" + to_string(7 * w_/sdl->width());
	auto& font = sdl->fonts().at(fontstring);
	string damage = to_string(damageTaken) + "%";
	if (damageTaken < 255)
	{
		if (r < 255) {
			r = damageTaken * 2;
			if (r > 255)r = 255;
		}
		else
		{
			g = 255 - damageTaken;
			if (g < 0)g = 0;
		}
	}
	else
	{
		Uint32 a = (((float)damageTaken - 255.f) / (float)SDL_MAX_SINT16) * 255 * 20;
		if (a < 255)
			r = 255 - a;
		else
			r = 0;
		g = 0;
	}
	Uint32 color = r * pow(16, 6) + g * pow(16, 4);
	SDL_Color c = build_sdlcolor(color);
	string fontstringp = "nes" + to_string(10 * w_ / sdl->width());
	auto& fontp = sdl->fonts().at(fontstringp);
	string key = fontstringp + damage + to_string(c.r) + to_string(c.g) + to_string(c.b);
	if (sdl->msgs().count(key) == 0) {
		sdl->msgs().emplace(key, Texture(sdl->renderer(), damage, fontp, c));
	}
	sdl->msgs().at(key).render(x+w_/15 , y+w_/22);

	//Numero jugador
	//
	string player = "Player " + to_string(playerPosition + 1);
	if (playerPosition == 0)c = build_sdlcolor(0xFF000000);
	else if (playerPosition == 1)c = build_sdlcolor(0x002EFF00);
	else if (playerPosition == 2)c = build_sdlcolor(0x00FF6100);
	else if (playerPosition == 3)c = build_sdlcolor(0xFFF00000);
	key = fontstring + player + to_string(c.r) + to_string(c.g) + to_string(c.b);
	if (sdl->msgs().count(key) == 0) {
		sdl->msgs().emplace(key, Texture(sdl->renderer(), player, font, c));
	}
	sdl->msgs().at(key).render(x, y);


	//Vidas
	//
	string vidas = "Lives:" + to_string(lives);
	c = build_sdlcolor(0x00F7FF00);
	key = fontstring + vidas + to_string(c.r) + to_string(c.g) + to_string(c.b);
	if (sdl->msgs().count(key) == 0) {
		sdl->msgs().emplace(key, Texture(sdl->renderer(), vidas, font, c));
	}
	sdl->msgs().at(key).render(x, y+w_/15);
}
