#include "Character.h"
#include "Utils/AnimationManager.h"

Character::Character(FightManager* manager) : Entity(manager)
{
	hurtbox = manager->GetSDLCoors(body, width, height);

	manager->GetWorld()->SetContactListener(&listener);

	ih.initialiseJoysticks();
}

Character::~Character()
{

}

void Character::update()
{

	Entity::update();


	SetGround();
	//std::cout << onGround << std::endl;

	//basicamente la cadena de ifs que estaba en el main
	ih.refresh();
	// exit when any key is down
	if (ih.isKeyDown(SDLK_d) || ih.getAxesState(0, 1) == 1)
	{
		speed = maxSpeed;
		moving = true;
		dir = 1;
	}
	if (ih.isKeyDown(SDLK_a) || ih.getAxesState(0, 1) == -1)
	{
		speed = -maxSpeed;
		moving = true;
		dir = -1;
	}
	if ((ih.isKeyUp(SDLK_a) && ih.isKeyUp(SDLK_d)) || ih.getAxesState(0, 1) == 0)
	{
		// para que no haya movimiento infinito (experimental)
		moving = false;

	}
	if ((ih.isKeyDown(SDLK_w) || ih.getAxesState(0, 4) == -1) && /*jumpCounter > 0 &&*/ currentMove == nullptr)
	{
		if (!GetGround())
		{
			jumpCounter--;
		}
		body->SetLinearVelocity(b2Vec2(speed, 0));
		body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpStr), true);
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
	if (currentMove == nullptr)
		body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));


	//Si se da la tecla del ataque y no hay un ataque en ejecucion...

	// Ataque con A (provisional)
	if ((ih.isKeyDown(SDLK_e) || ih.getButtonState(0, 1)) && currentMove == nullptr && onGround)
	{
		//paramos al personaje
		body->SetLinearVelocity(b2Vec2(0, 0));

		//Declaramos el valor del ataque como el ataque que queramos
		currentMove = &Character::BasicNeutral;
	}

	// Ataque con B (provisional)
	if ((ih.isKeyDown(SDLK_r) || ih.getButtonState(0, 2)) && currentMove == nullptr && onGround)
	{
		currentMove = &Character::SpecialNeutral;
	}

	if (ih.isKeyDown(SDLK_p))
	{
		std::cout << "onGround?" << onGround << std::endl;
		std::cout << "listener ground?" << listener.CheckGround() << std::endl;
	}

	// Botones START Y SELECT (de momento solo hacen cout)
	if (ih.getButtonState(0, 8))std::cout << "SELECT PRESSED" << std::endl;

	if (ih.getButtonState(0, 9))std::cout << "START PRESSED" << std::endl;

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
}

void Character::SetGround()
{
	onGround = listener.CheckGround();
}

void Character::draw()
{
	//xd
	//Este rect representa el rectangulo en la spritesheet en si que contiene el sprite concreto que queremos
	//Cambiad los 0 por numeros entre 0 y 3 para cambiar la posicion del rect y por ende el sprite
	//Los numeros son en pixeles en la imagen

	anim->update();



	Entity::draw();
}


void Character::GetHit(atackData a, int opdir)
{
	//Actualiza el daño
	damageTaken += a.damage;
	//Produce el knoback..
	body->SetLinearVelocity(b2Vec2((a.multiplier * damageTaken) * opdir, -a.multiplier * damageTaken));
}



SDL_Rect* Character::GetHurtbox()
{
	return &hurtbox;
}
