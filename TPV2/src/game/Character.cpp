#include "Character.h"

Character::Character(FightManager* manager, bool movable) : Entity(manager)
{

	this->movable = movable;

	texture = &sdl->images().at("makt");

	//Aqui defino las caracteristicas de cada hitbox (podriamos hacerlo dentro de cada metodo, y vendria de json)(tambien podr�amos poner framedata)
	ataqueFuerte.damage = 20;
	ataqueFuerte.multiplier = 1.8f;

	ataqueDebil.damage = 5;
	ataqueDebil.multiplier = 0.2f;

	// variables
	damageTaken = 0;
	maxSpeed = 40;
	speed = 0;
	maxJumps = 1;
	jumpStr = 10000;
	jumpCounter = maxJumps;
	onGround = true;

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

	//este bool lo puse para el personaje de prueba xd
	if (movable)
	{
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
			currentMove = &Character::atackStrong;
		}

		// Ataque con B (provisional)
		if ((ih.isKeyDown(SDLK_r) || ih.getButtonState(0, 2)) && currentMove == nullptr && onGround)
		{
			currentMove = &Character::atackWeak;
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
}

void Character::atackStrong(int frameNumber)
{

	//Dependiendo del frame en el que est�, hara una cosa u otra..

	switch (frameNumber)
	{
	case 0:
		//Empieza el ataque :v
		//No hace nada, esto es el cargar el pu�o
		break;
	case 90:
	{

		//Al frame 90, crea un rect y si el oponente colisiona con ello...
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w /= 2;
		hitbox.h /= 2;

		hitbox.x += hitbox.w / 2;

		hitbox.y += hitbox.h / 2;

		hitbox.x += dir * 60;

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		for (int i = 0; i < oponents.size(); i++)
		{
			if (SDL_HasIntersection(&hitbox, oponents[i]->GetHurtbox()))
			{
				//Le hace da�o xddd
				oponents[i]->GetHit(ataqueFuerte, dir);
			}
		}
	}
		break;
	case 160:

		//Al ultimo frame...

		//Vacia current move para que Character sepa que ha acabado
		currentMove = nullptr;

		//Reinicia moveFrame para el siguiente
		moveFrame = -1;
		break;
	}
}

//Lo mismo que el de arriba pero mas rapido y debil xd
void Character::atackWeak(int frameNumber)
{
	switch (frameNumber)
	{
	case 12:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 30;

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		for (int i = 0; i < oponents.size(); i++)
		{
			if (SDL_HasIntersection(&hitbox, oponents[i]->GetHurtbox()))
			{
				//Le hace da�o xddd
				oponents[i]->GetHit(ataqueDebil, dir);
			}
		}
	}
	break;
	case 20:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}
void Character::SetGround()
{
	onGround = listener.CheckGround();
}

void Character::draw()
{
	Entity::draw();

	texture->render(hurtbox.x, hurtbox.y);
}


void Character::GetHit(atackData a, int dir)
{
	//Actualiza el da�o
	damageTaken += a.damage;
	//Produce el knoback..
	body->SetLinearVelocity(b2Vec2((a.multiplier * damageTaken) * dir, -a.multiplier * damageTaken));
}



SDL_Rect* Character::GetHurtbox()
{
	return &hurtbox;
}
