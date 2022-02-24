#include "Character.h"

Character::Character(b2World* world, SDLUtils* sdl, bool movable, SDL_Texture* texture) : Entity(world, sdl, texture)
{

	this->movable = movable;

	//Aqui defino las caracteristicas de cada hitbox (podriamos hacerlo dentro de cada metodo, y vendria de json)(tambien podríamos poner framedata)
	ataqueFuerte.damage = 20;
	ataqueFuerte.multiplier = 1.8f;

	ataqueDebil.damage = 5;
	ataqueDebil.multiplier = 0.2f;

	// variables
	damageTaken = 0;
	maxSpeed = 40;
	speed = 0;
	this->maxJumps = 1;
	this->jumpStr = 4500;
	jumpCounter = maxJumps;
	oponent = nullptr;
	onGround = true;

	//creamos el detector de colisiones
	world->SetContactListener(&listener);
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
		if (ih.isKeyDown(SDLK_d))
		{
			speed = maxSpeed;
			moving = true;
			dir = 1;
		}
		if (ih.isKeyDown(SDLK_a))
		{
			speed = -maxSpeed;
			moving = true;
			dir = -1;
		}
		if (ih.isKeyUp(SDLK_a) && ih.isKeyUp(SDLK_d))
		{
			// para que no haya movimiento infinito (experimental)
			moving = false;

		}
		if (ih.isKeyDown(SDLK_w) && /*jumpCounter > 0 &&*/ currentMove == nullptr)
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

		if (ih.isKeyDown(SDLK_e) && currentMove == nullptr && onGround)
		{
			//paramos al personaje
			body->SetLinearVelocity(b2Vec2(0, 0));

			//Declaramos el valor del ataque como el ataque que queramos
			currentMove = &Character::atackStrong;
		}

		if (ih.isKeyDown(SDLK_r) && currentMove == nullptr && onGround)
		{
			body->SetLinearVelocity(b2Vec2(0, 0));

			currentMove = &Character::atackWeak;
		}
		if (ih.isKeyDown(SDLK_p))
		{
			std::cout << "onGround?" << onGround << std::endl;
			std::cout << "listener ground?" << listener.CheckGround() << std::endl;
		}


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

	//Dependiendo del frame en el que esté, hara una cosa u otra..

	switch (frameNumber)
	{
	case 0:
		//Empieza el ataque :v
		//No hace nada, esto es el cargar el puño
		break;
	case 90:
	{

		//Al frame 90, crea un rect y si el oponente colisiona con ello...
		SDL_Rect hitbox = { 
			(int)(body->GetPosition().x * 20 - width * 5 + dir * 50),
			(int)(body->GetPosition().y * 20 - height * 5), 
			(int)width * 10, 
			(int)height * 10 };

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		if (SDL_HasIntersection(&hitbox, oponent->GetHurtbox()))
		{
			//Le hace daño xddd
			oponent->GetHit(ataqueFuerte, dir);
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
		SDL_Rect hitbox = {
			(int)(body->GetPosition().x * 20.f - width * 10 + dir * 30),
			(int)(body->GetPosition().y * 20.f - height * 10),
			(int)width * 20,
			(int)height * 20 };

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		if (SDL_HasIntersection(&hitbox, oponent->GetHurtbox()))
		{
			oponent->GetHit(ataqueDebil, dir);
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
}


void Character::GetHit(atackData a, int dir)
{
	//Actualiza el daño
	damageTaken += a.damage;
	//Produce el knoback..
	body->SetLinearVelocity(b2Vec2((a.multiplier * damageTaken) * dir, -a.multiplier * damageTaken));
}


//Le decimos a quien toca dar de ostias xd
void Character::SetOponent(Character* op)
{
	oponent = op;
}

SDL_Rect* Character::GetHurtbox()
{
	return &hurtbox;
}
