#include "Character.h"
#include "Utils/AnimationManager.h"
#include "Utils/InputConfig.h"


Character::Character(FightManager* manager, Vector2D* pos, char input) : 
	Entity(manager, pos)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	stun = 0;
	shieldCounter = maxShield = 60;
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
		if (input->basic() && onGround)
		{
			//paramos al personaje
			body->SetLinearVelocity(b2Vec2(0, 0));

			//Declaramos el valor del ataque como el ataque que queramos
			currentMove = &Character::BasicNeutral;
		}
		//básico en movimiento
		if (input->basic() && (input->right() || input->left()) && onGround)
		{
			//Declaramos el valor del ataque como el ataque que queramos
			currentMove = &Character::BasicForward;
		}
		//básico abajo
		if (input->basic() && input->down() && onGround )
		{
			//Declaramos el valor del ataque como el ataque que queramos
			currentMove = &Character::BasicDownward;
		}

		// Ataque con B (provisional)
		if (input->special() && onGround)
		{
			currentMove = &Character::SpecialNeutral;
		}
		if (input->down() && onGround && shieldCounter > 0) {

			currentMove = &Character::StartShield;
			shieldCounter--;
			std::cout << shield << endl;
			std::cout << shieldCounter << endl;
			body->SetLinearVelocity(b2Vec2(0, 0));

		}
		else if (input->stop())
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

			currentMove = &Character::Dash;
		}
	}


	//para recuperar escudo
	if (currentMove != &Character::StartShield && shieldCounter < maxShield)
	{
		shieldCounter++;
	}
	else if (currentMove == &Character::StartShield)
	{
		shieldCounter--;
	}


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

	//Si se da la tecla del ataque y no hay un ataque en ejecucion...


	//// Botones START Y SELECT (de momento solo hacen cout)
	//if (ih.getButtonState(0, 8))std::cout << "SELECT PRESSED" << std::endl;

	//if (ih.getButtonState(0, 9))std::cout << "START PRESSED" << std::endl;

	// Util para saber que buttonNumber es cada boton del mando
	//for (int i = 0; i < 10; i++)if (ih.getButtonState(0, i))std::cout << i << std::endl;

	//Si hay un movimiento en ejecucion lo continuamos...
	if (currentMove != nullptr)
	{
		//ejecuta el ataque guardado en la variable
		(*this.*currentMove)(moveFrame);
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
		anim->StartAnimation(3);
		shield = true;
	}
	if (!input->down() || shieldCounter <= 0 || (input->basic()||input->special()))
	{
		currentMove = &Character::EndShield;
	}
}
void Character::EndShield(int frameNumber)
{
	anim->StartAnimation(0);
	currentMove = nullptr;
	moveFrame = -1;
	shield = false;
	if (input->basic() && onGround && input->down())
	{
		currentMove = &Character::BasicDownward;
	}
	else if(input->special() && onGround && input->down())
	{
		currentMove = &Character::SpecialDownward;
	}
}

void Character::Dash(int frameNumber)
{

	switch (frameNumber)
	{
	case 0:
		anim->StartAnimation(3);
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
		anim->StartAnimation(0);
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

	anim->StartAnimation(0);
}


