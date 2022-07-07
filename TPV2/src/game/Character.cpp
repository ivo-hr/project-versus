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
	shieldCounter = maxShield;
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

		std::vector<int> a = animData[i]["keySprite"];
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

Character::Character(FightManager* manager, b2Vec2 pos, char input,int playerPos, float w, float h) :
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
	shield = false;
	lives = maxLives;
	this->input = new InputConfig(input);
	input_ = input;
	totalDamageTaken = 0;
	kills = 0;
	resetLastCharacter();
}

Character::~Character()
{
	r = 0;
	g = 255;
	delete input;
	delete anim;
}

void Character::SetSpawn(b2Vec2 spawn, int dir)
{
	body->SetTransform(spawn, 0);
	this->dir = dir;
	respawnPos = b2Vec2(spawn.x, 5);
}

void Character::SetPNumber(uint16 num)
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

	if (hitLag > 0)
	{
		hitLag--;
		if (hitLag == 0)
		{
			body->SetEnabled(true);
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


	if (stun > 0) {
		if (anim->CurrentAnimation() != "stun" + animAddon)
			anim->StartAnimation("stun" + animAddon);

		if (input->right())
		{
			body->ApplyLinearImpulseToCenter({ 5, 0 }, true);
		}
		if (input->left())
		{
			body->ApplyLinearImpulseToCenter({ -5, 0 }, true);
		}

		currentMove = nullptr;
	}
	else
	{ 
		if (currentMove == nullptr || (currentMove != nullptr && !onGround))
		{
			if (input->right() && input->left())
			{
				speed = 0;
			}
			else
			{

				if (input->right())
				{
					if (currentMove == nullptr)
						dir = 1;

					if (speed < 1)
						AddParticle(new Particle(Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, "run", this));

					speed = maxSpeed;
				}
				if (input->left())
				{
					if (currentMove == nullptr)
						dir = -1;

					if (speed > -1)
						AddParticle(new Particle(Vector2D(hurtbox.x + hurtbox.w / 2, hurtbox.y + hurtbox.h), dir, "run", this));

					speed = -maxSpeed;
				}
			}
		}

	}
	if (!recovery) {
		if (anim->CurrentAnimation() != "dash" + animAddon)
			anim->StartAnimation("dash" + animAddon);

	}

	if (speed > 4)
		speed -= 4;
	else if (speed < -4)
		speed += 4;
	else
		speed = 0;

	if (currentMove == nullptr /*|| currentMove == Taunt())*/ && stun == 0 && recovery)
	{

		// Ataque con A (provisional)
		
		if (input->basic())
		{
			sdl->soundEffects().at(codeName + "Steps").haltChannel();

			if (input->up()) //básico arriba
			{
				StartMove([this](int f) { BasicUpward(f); });
			}
			else if (input->down()) //básico abajo
			{
				StartMove([this](int f) { BasicDownward(f); });
			}
			else if (input->right() || input->left()) //básico en movimiento
			{
				StartMove([this](int f) { BasicForward(f); });
			}
			else //básico estático
			{
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
				StartMove([this](int f) { SpecialUpward(f); });
			}
			else if (input->down()) //especial abajo
			{
				StartMove([this](int f) { SpecialDownward(f); });
			}
			else if (input->right() || input->left()) //especial en movimiento
			{
				StartMove([this](int f) { SpecialForward(f); });
			}
			else //especial estático
			{
				StartMove([this](int f) { SpecialNeutral(f); });
			}

			manager->MoveToFront(this);

		}

		//Escudo
		if (input->down() && onGround && shieldCounter > (maxShield/3) && (body->GetLinearVelocity().y > -0.1f && body->GetLinearVelocity().y < 0.1f)) {

			StartMove([this](int f) { StartShield(f); });
			body->SetLinearVelocity(b2Vec2(0, 0));

		}
		else if (input->down() && !onGround)
		{
			StartMove([this](int f) { Dash(f); });
		}

		// salto
		if (input->up() && !(jumpCounter <= 0 || !jumpCooldown))
		{
			StartMove([this](int f) { StartJump(f); });
		}

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

		// bajar plataformas
		if (down && input->downReleased()) {
			down = false;
			fall = maxFallCount; // Activa contador para reconocer el bajar plataformas
		}

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
		if (input->taunt() && onGround) 
		{
			sdl->soundEffects().at(codeName + "Taunt").play();

			StartMove([this](int f) { Taunt(f); });
		}
	}

	//else sdl->soundEffects().at(codeName + "Steps").haltChannel();

	if (input->down() && body->GetFixtureList()->GetFilterData().maskBits != 2) down = true; // Marca que se ha pulsado abajo (para el tema de bajar plataformas)

	if (reactivateColl > 0) reactivateColl--;
	if (reactivateColl == 0 && body->GetFixtureList()->GetFilterData().maskBits == 2) { // Tras medio segundo reactiva colisión jugador-plataformas
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			auto fix = f->GetFilterData();
			fix.maskBits = 2 | 4;
			f->SetFilterData(fix);
		}
	}
	//para recuperar escudo
	if (!shield && shieldCounter < maxShield)
	{
		shieldCounter++;
	}
	else if (shield)
	{
		shieldCounter-=2;
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


	//Si hay un movimiento en ejecucion lo continuamos...
	if (currentMove != nullptr)
	{
		//ejecuta el ataque guardado en la variable
		(currentMove)(moveFrame);
		//Actualiza el frame actual del movimiento
		moveFrame++;
	}

	// Efectos de estado
	if (efEstado != none && stateCont < stateDur)
	{
		stateCont++;
		if ((efEstado == fire|| efEstado == wElectric) && stateCont % 60 == 0)
		{
			Elements();
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

	anim->update();

	hurtbox.x = manager->b2ToSDLX(body, width);
	hurtbox.y = manager->b2ToSDLY(body, height);

	if (!SDL_HasIntersection(&hurtbox, manager->GetDeathZone()))
	{
		OnDeath();
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

	if (shield)
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
	if (drawArrow) {
		Entity::draw(camera);

		if (!alive) return;

		anim->render(camera);

	}
	SDL_Rect aux = hurtbox;

	aux.x -= camera->x;
	aux.x *= (manager->GetActualWidth() / (float)camera->w);

	aux.y -= camera->y;
	aux.y *= (manager->GetActualHeight() / (float)camera->h);

	aux.w *= (manager->GetActualWidth() / (float)camera->w);
	aux.h *= (manager->GetActualHeight() / (float)camera->h);

	int xpos = aux.x + (aux.w / 2);

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

	arrowsTex->render(arrowSrc, { xpos - 15, aux.y - 44, 30, 16 });

#ifdef _DEBUG

	if (shield)
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


bool Character::GetHit(HitData a, Entity* attacker)
{
	if (shield)
	{
		//Actualiza el da�o
		if (!a.shieldBreak)
		{
			damageTaken += (int)(a.damage * 0.4f);
		}
		else
		{
			shield = false;
			body->SetGravityScale(10.0f);
			currentMove = nullptr;
			moveFrame = -1;
			anim->StartAnimation("stun" + animAddon);
			anim->update();
			float recoil = ((a.base * 2) + ((damageTaken * a.multiplier) / (weight * .2f)));

			stun = a.GetStun(recoil);

			//Actualiza el da�o
			damageTaken += a.damage * 2;

			b2Vec2 aux = a.direction;

			if (recoil > 90)
			{
				manager->KillingBlow();
				AddHitLag(50);
				attacker->AddHitLag(50);

				AddParticle(new Particle(
					Vector2D(
						manager->ToSDL(body->GetPosition().x),
						manager->ToSDL(body->GetPosition().y)),
					1, "killVfx", this));
				AddParticle(new Particle(
					Vector2D(
						manager->ToSDL(body->GetPosition().x),
						manager->ToSDL(body->GetPosition().y)),
					1, "killHit", this));
			}
			else
			{
				AddHitLag(30);
				attacker->AddHitLag(30);

				AddParticle(new Particle(
					Vector2D(
						manager->ToSDL(body->GetPosition().x),
						manager->ToSDL(body->GetPosition().y)),
					1, "shieldBroken", this));
			}

			aux *= recoil;
			aux.y *= -1;
			aux.x *= attacker->GetDir();

			//Produce el knoback..
			body->SetLinearVelocity(aux);
			shieldCounter = 0;
		}
		return true;
	}
	if (dash)
	{
		return false;
	}
	else if (!shield && !dash && !attacker->ToDelete())
	{
		body->SetGravityScale(10.0f);
		currentMove = nullptr;
		moveFrame = -1;
		anim->StartAnimation("stun" + animAddon);
		anim->update();
		float recoil = (a.base + ((damageTaken * a.multiplier) / (weight * .2f)));

		stun = a.GetStun(recoil);

		//Actualiza el da�o
		damageTaken += a.damage;
		
		b2Vec2 aux = a.direction;

		if (recoil > 90)
		{
			manager->KillingBlow();
			AddHitLag(40);
			attacker->AddHitLag(40);

			AddParticle(new Particle(
				Vector2D(
					manager->ToSDL(body->GetPosition().x),
					manager->ToSDL(body->GetPosition().y)),
				1, "killVfx", this));
			AddParticle(new Particle(
				Vector2D(
					manager->ToSDL(body->GetPosition().x),
					manager->ToSDL(body->GetPosition().y)),
				1, "killHit", this));
		}

		aux *= recoil;
		aux.y *= -1;
		aux.x *= attacker->GetDir();

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
					auto plasma = new Explosion(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - height / 2), poder, 1);
					manager->AddEntity(plasma);
					plasma->SetOponents(manager->GetEntities(plasma));
					manager->MoveToFront(plasma);
					efEstado = none;
					statePower = 0;
					stateCont = 0;
				}
				//fuego y agua
				else if((efEstado == fire && a.estado == water) || (efEstado == water && a.estado == fire))
				{
					if (efEstado == water)
					{
						maxSpeed += ralentizar;
						ralentizar = 0;
					}
					int poder = (statePower + a.power) / 2;
					auto vapor = new Explosion(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - height / 2), poder, 0);
					manager->AddEntity(vapor);
					vapor->SetOponents(manager->GetEntities(vapor));
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
					stun += statePower * 1.5;
				}
			}
			//efecto de estado básico
			else if (efEstado == none)
			{
				efEstado = a.estado;
				statePower = a.power;
				if (efEstado == electric)
				{
					stun += statePower * 1.5;

					AddParticle(new Particle(Vector2D(hurtbox.x, hurtbox.y), 1, "electric", this));
				}
				else if (efEstado == water)
				{
					ralentizar = maxSpeed * ((float)statePower / 100);
					maxSpeed -= ralentizar;

					AddParticle(new Particle(Vector2D(hurtbox.x, hurtbox.y), 1, "water", this));
				}
				else if (efEstado == fire)
				{
					AddParticle(new Particle(Vector2D(hurtbox.x, hurtbox.y), 1, "fire", this));
				}
			}
		}

		//Produce el knoback..
		body->SetLinearVelocity(aux);

		return true;
	}

}


void Character::StartJump(int frameNumber)
{
	if (jumpCounter <= 0 || !jumpCooldown)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (frameNumber < 3)
	{
		anim->StartAnimation("jumpCharge" + animAddon);
		if (input->right())
		{
			speed = maxSpeed;
			dir = 1;
		}
		if (input->left())
		{
			speed = -maxSpeed;
			dir = -1;
		}
		if (input->right() && input->left())
		{
			speed = 0;
		}

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
			else if (input->down()) //básico abajo
			{
				ChangeMove([this](int f) { BasicDownward(f); });
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
			body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);

		}
		else if (input->special())
		{

			if (input->up()) //especial arriba
			{
				ChangeMove([this](int f) { SpecialUpward(f); });
			}
			else if (input->down()) //especial abajo
			{
				ChangeMove([this](int f) { SpecialDownward(f); });
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
			body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);

		}
		else
		{
			if (input->up())
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
			else
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr * 0.6f), true);
		}

		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Character::StartShield(int frameNumber)
{
	if (frameNumber == 1)
	{
		sdl->soundEffects().at("shield").play();

		anim->StartAnimation("shield" + animAddon);
		shield = true;
	}
	if (!input->down() || shieldCounter <= 0 )
	{
		ChangeMove([this](int f) { EndShield(f); });
	}
	if (input->basic())
	{
		shield = false;
		ChangeMove([this](int f) { BasicDownward(f); });
	}
	else if (input->special())
	{
		shield = false;
		ChangeMove([this](int f) { SpecialDownward(f); });
	}
}
void Character::EndShield(int frameNumber)
{
	anim->StartAnimation("idle" + animAddon);
	currentMove = nullptr;
	moveFrame = -1;
	shield = false;
}

void Character::Dash(int frameNumber)
{

	switch (frameNumber)
	{
	case 0:
		anim->StartAnimation("dash" + animAddon);
		dash = true;
		body->SetLinearVelocity(b2Vec2(0, 20));
		break;
	case 60:
		dash = false;
		currentMove = nullptr;
		anim->StartAnimation("idle" + animAddon);
		break;
	}
	if (onGround)
	{
		dash = false;
		currentMove = nullptr;
		anim->StartAnimation("idle");
	}
}

void Character::StartMove(std::function<void(int)> newMove)
{
	currentMove = newMove;
	moveFrame = 0;
}
void Character::ChangeMove(std::function<void(int)> newMove)
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

	//Canal 1 , (antes a veces no se escucha)
	sdl->soundEffects().at("death").play(0,1);

	alive = false;
	lives--;

	AddDeathParticle();
	body->SetTransform(respawnPos, 0);
	body->SetAwake(false);
	currentMove = nullptr;
	moveFrame = 0;
	moving = false;
	shield = false;
	dash = false;
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
	if (lives == 0)
		return;
	//O dios mio que he creado
	AddParticle(new Particle(
		Vector2D(
			( manager->ToSDL(body->GetPosition().x) < 0 ?
				0 : manager->ToSDL(body->GetPosition().x )) > manager->GetDeathZone()->w?
				manager->GetDeathZone()->w : manager->ToSDL(body->GetPosition().x),

			( manager->ToSDL(body->GetPosition().y) < 0 ? 
				0 : manager->ToSDL(body->GetPosition().y )) > manager->GetDeathZone()->h ?
				manager->GetDeathZone()->h : manager->ToSDL(body->GetPosition().y)),
		1, "died", this));
}

void Character::Respawn()
{
	body->SetAwake(true);

	std::cout << "Vidas restantes: " << lives << "\n";

	body->SetLinearVelocity({ 0, 0 }); // resetea la velocidad
	speed = 0;

	alive = true;
	totalDamageTaken += damageTaken;
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

void Character::Taunt(int frameNumber) {
	if (frameNumber == 1)
	{
		anim->StartAnimation("taunt" + animAddon);
	}
	if (input->right())
	{
		speed = maxSpeed;
		dir = 1;
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (input->left())
	{
		speed = -maxSpeed;
		dir = -1;
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (input->right() && input->left())
	{
		speed = 0;
		currentMove = nullptr;
		moveFrame = -1;
	}
	if (input->basic())
	{

		if (input->up()) //básico arriba
		{
			ChangeMove([this](int f) { BasicUpward(f); });
		}
		else if (input->down()) //básico abajo
		{
			ChangeMove([this](int f) { BasicDownward(f); });
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

	}

	// Ataque con B (provisional)
	if (input->special())
	{

		if (input->up()) //especial arriba
		{
			ChangeMove([this](int f) { SpecialUpward(f); });
		}
		else if (input->down()) //especial abajo
		{
			ChangeMove([this](int f) { SpecialDownward(f); });
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

	}

	if (input->down() && onGround && shieldCounter > (maxShield / 3) && (body->GetLinearVelocity().y > -0.1f && body->GetLinearVelocity().y < 0.1f)) {

		ChangeMove([this](int f) { StartShield(f); });
		body->SetLinearVelocity(b2Vec2(0, 0));

	}
	// salto
	if (input->up() && !(jumpCounter <= 0 || !jumpCooldown))
	{
		ChangeMove([this](int f) { StartJump(f); });
	}

	else if (frameNumber == attacks["taunt"].totalFrames)
	{
		anim->StartAnimation("idle" + animAddon);
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
		stun += (statePower / (stateDur / 60))*1.5;
		body->SetLinearVelocity({ 0, 0 });
	}
}
void Character::drawHUD(int numOfPlayer)
{
	//Portrait y posiciones
	int w_ = manager->GetDeathZone()->w;
	int h_ = manager->GetDeathZone()->h;
	int dist = w_/ numOfPlayer;
	int offset = (w_ / 2) / numOfPlayer - w_/30;
	int x = (int)(playerPosition * dist + offset);
	int y = h_ - (h_ / 6);
	portrait->render({ x, y, w_ / 15, w_ / 15 });
	//Porcentaje
	string fontstring = "nes" + to_string(7 * w_/sdl->width());
	auto& font = sdl->fonts().at(fontstring);
	string damage = to_string(damageTaken) + "%";
	if (r < 255) {
		r = damageTaken * 2;
		if (r > 255)r = 255;
	}
	else
	{
		g = 255 - damageTaken;
		if (g < 0)g = 0;
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
	string player = "Player" + to_string(playerPosition + 1);
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
