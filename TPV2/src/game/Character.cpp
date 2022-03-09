#include "Character.h"
#include "Utils/AnimationManager.h"
#include "Utils/InputConfig.h"


Character::Character(FightManager* manager,char input) : Entity(manager)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	stun = 0;

	manager->GetWorld()->SetContactListener(&listener);
	this->input = new InputConfig(input);
}

Character::~Character()
{

}

void Character::update()
{

	if (stun > 0)
		stun--;

	SetGround();
	
	if (input->right())
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
	
	else if (input->up() && currentMove == nullptr)
	{
		if (!GetGround())
		{
			jumpCounter--;
		}
		body->SetLinearVelocity(b2Vec2(speed, 0));
		body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
	}
	// Ataque con A (provisional)
	else if (input->basic() && currentMove == nullptr && onGround)
	{
		//paramos al personaje
		body->SetLinearVelocity(b2Vec2(0, 0));

		//Declaramos el valor del ataque como el ataque que queramos
		currentMove = &Character::BasicNeutral;
	}

	// Ataque con B (provisional)
	else if (input->special() && currentMove == nullptr && onGround)
	{
		currentMove = &Character::SpecialNeutral;
	}
	else if (input->stop())
	{
		// para que no haya movimiento infinito (experimental)
		moving = false;

	}
	//boolean to check collision with the ground
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

void Character::SetGround()
{
	onGround = listener.CheckGround();
}

void Character::draw()
{
	//xd

	Entity::draw();
}


void Character::GetHit(atackData a, int opdir)
{

	float recoil = (a.base + ((damageTaken * a.multiplier) / (weight * .2f)));

	if (a.base >= 0)
	{
		stun = recoil / 1.8f;
	}

	//Actualiza el daño
	damageTaken += a.damage;

	b2Vec2 aux = a.direction;

	aux *= recoil;
	aux.y *= -1;
	aux.x *= opdir;

	//Produce el knoback..
	body->SetLinearVelocity(aux);
}



SDL_Rect* Character::GetHurtbox()
{
	return &hurtbox;
}


