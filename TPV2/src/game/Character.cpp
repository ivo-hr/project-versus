#include "Character.h"
#include "Utils/AnimationManager.h"
#include "Utils/InputConfig.h"


Character::Character(FightManager* manager, Vector2D* pos, char input, float w, float h) :
	Entity(manager, pos, w, h)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	stun = 0;
	dash = false;
	lives = 3;

	this->input = new InputConfig(input);
}

Character::~Character()
{

}

void Character::update()
{

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
		stun--;

	if (currentMove == nullptr && stun == 0)
	{
		if (input->right() && input->left())
		{
			if (speed > 0) {
				speed = -maxSpeed;
				moving = true;
				dir = -1;
			}
			else if (speed < 0) {
				speed = maxSpeed;
				moving = true;
				dir = 1;
			}
			else moving = false;

		}
		else if (input->right())
		{
			speed = maxSpeed;
			moving = true;
			dir = 1;
		}
		else if (input->left())
		{
			speed = -maxSpeed;
			moving = true;
			dir = -1;
		}

		// Ataque con A (provisional)
		//básico estático
		if (input->basic())
		{

			if (input->right() || input->left())
			{
				currentMove = [this](int f) { BasicForward(f); };
			}
			else if (input->down())
			{
				currentMove = [this](int f) { BasicDownward(f); };
			}
			else if (input->up())
			{
				currentMove = [this](int f) { BasicUpward(f); };
			}
			else
			{
				currentMove = [this](int f) { BasicNeutral(f); };
			}

		}

		// Ataque con B (provisional)
		if (input->special())
		{

			if (input->right() || input->left())
			{
				currentMove = [this](int f) { SpecialForward(f); };
			}
			else if (input->down())
			{
				currentMove = [this](int f) { SpecialDownward(f); };
			}
			else if (input->up())
			{
				currentMove = [this](int f) { SpecialUpward(f); };
			}
			else
			{
				currentMove = [this](int f) { SpecialNeutral(f); };
			}

		}

		//Escudo
		if (input->down() && onGround && shieldCounter > (maxShield/3)) {

			currentMove = [this](int f) { StartShield(f); };
			shieldCounter--;
			std::cout << shield << endl;
			std::cout << shieldCounter << endl;
			body->SetLinearVelocity(b2Vec2(0, 0));

		}



		if (input->stop())
		{
			// para que no haya movimiento infinito (experimental)
			moving = false;

		}


		if (input->up())
		{
			if (jumpCounter > 0) {
				if (!GetGround())
				{
					jumpCounter--;
				}
				body->SetLinearVelocity(b2Vec2(speed, 0));
				body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
			}
		}
		
		//dash
		if (input->down() && !onGround) {

			currentMove = [this](int f) { Dash(f); };
		}
	}


	//para recuperar escudo
	if (!shield && shieldCounter < maxShield)
	{
		std::cout << shieldCounter << endl;
		shieldCounter++;
	}
	else if (shield)
	{
		std::cout << shieldCounter << endl;
		shieldCounter-=2;
	}

	//Chequeo de tierra
	if (GetGround())
	{
		jumpCounter = maxJumps;
	}


	if (!moving)
	{
		if (speed > 0)
			speed -= 2;
		if (speed < 0)
			speed += 2;
	}

	//Que se mueva si no esta haciendo un ataque ya
	if (currentMove == nullptr && stun == 0)
		body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));


	//Si hay un movimiento en ejecucion lo continuamos...
	if (currentMove != nullptr)
	{
		//ejecuta el ataque guardado en la variable
		(currentMove)(moveFrame);
		//Actualiza el frame actual del movimiento
		moveFrame++;
	}

	Entity::update();

}

void Character::draw()
{
	//xd

	if (!alive) return;

	Entity::draw();

	//if (debug)

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
}


bool Character::GetHit(attackData a, int opdir)
{
	std::cout << shield << endl;
	if (shield)
	{
		//Actualiza el da�o
		damageTaken += (int)(a.damage * 0.4f);
		return true;
	}
	if (dash)
	{
		return false;
	}
	else if (!shield && !dash)
	{
		float recoil = (a.base + ((damageTaken * a.multiplier) / (weight * .2f)));

		if (a.base >= 0)
		{
			stun = recoil / 1.8f;
		}

		//Actualiza el da�o
		damageTaken += a.damage;
		
		b2Vec2 aux = a.direction;

		aux *= recoil;
		aux.y *= -1;
		aux.x *= opdir;

		//Produce el knoback..
		body->SetLinearVelocity(aux);

		return true;
	}

}
void Character::StartShield(int frameNumber)
{
	if (frameNumber == 1)
	{
		anim->StartAnimation("shield");
		shield = true;
	}
	if (!input->down() || shieldCounter <= 0 )
	{
		currentMove = [this](int f) { EndShield(f); };
	}
	if (input->basic())
	{
		moveFrame = -1;
		shield = false;
		currentMove = [this](int f) { BasicDownward(f); };
	}
	else if (input->special())
	{
		moveFrame = -1;
		shield = false;
		currentMove = [this](int f) { SpecialDownward(f); };
	}
}
void Character::EndShield(int frameNumber)
{
	anim->StartAnimation("idle");
	currentMove = nullptr;
	moveFrame = -1;
	shield = false;
}

void Character::Dash(int frameNumber)
{

	switch (frameNumber)
	{
	case 0:
		anim->StartAnimation("dash");
		dash = true;
		body->SetLinearVelocity(b2Vec2(0, 500));
		break;
	case 60:
		dash = false;
		currentMove = nullptr;
		moveFrame = -1;
		anim->StartAnimation(0);
		break;
	}
	if (onGround)
	{
		dash = false;
		currentMove = nullptr;
		moveFrame = -1;
		anim->StartAnimation("idle");
	}
}

SDL_Rect* Character::GetHurtbox()
{
	return &hurtbox;
}

void Character::OnDeath()
{
	body->SetAwake(false);
	alive = false;
	lives--;
	currentMove = nullptr;
	moveFrame = 0;
	moving = false;
	shield = false;
	dash = false;
}

void Character::Respawn()
{
	body->SetAwake(true);

	std::cout << "Vidas restantes: " << lives << "\n";

	body->SetTransform({ respawnPos.getX(), respawnPos.getY() }, 0);
	body->SetLinearVelocity({ 0, 0 }); // resetea la velocidad
	speed = 0;

	alive = true;
	damageTaken = 0;
	moving = false;

	currentMove = nullptr;
	moveFrame = 0;

	anim->StartAnimation("idle");
}


