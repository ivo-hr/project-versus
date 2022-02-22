#include "Character.h"

Character::Character(b2World* world, SDLUtils* sdl, bool movable)
{

	this->movable = movable;

	this->sdl = sdl;

	//Definimos un objeto (dinámico)

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(80.0f, 45.0f);
	groundBodyDef.type = b2_dynamicBody;

	//Definimos un caja
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width / 2, height / 2);

	//Creamos una "cuerpo" 
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 10.f;
	fixtureDef.friction = 0.9f;

	body = world->CreateBody(&groundBodyDef);
	//añadimos el cuerpo al objeto fisico
	body->CreateFixture(&fixtureDef);

	body->SetFixedRotation(true);

	body->SetGravityScale(10.f);

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
	this->jumpStr = 20000;
	oponent = nullptr;
	onGround = true;

	//Tamaño de la hurtbox del personaje
	hurtbox = { (int)(body->GetPosition().x - width), (int)(body->GetPosition().y - height), (int)width * 10, (int)height * 10 };

	//creamos el detector de colisiones
	world->SetContactListener(&listener);
}

Character::~Character()
{

}

void Character::update()
{
	//este bool lo puse para el personaje de prueba xd
	if (movable)
	{

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
		if (ih.isKeyDown(SDLK_w) && jumpCounter > 0 && currentMove == nullptr)
		{
			std::cout << " OnGround?: " << GetGround() << std::endl;
			std::cout << " jumps?: " << jumpCounter << std::endl;
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


		//Si hay un movimiento en ejecucion lo continuamos...
		if (currentMove != nullptr)
		{
			//ejecuta el ataque guardado en la variable
			(*this.*currentMove)(moveFrame);
			//Actualiza el frame actual del movimiento
			moveFrame++;
		}

	}

	//Actualizamos la posicion del rect
	hurtbox.x = body->GetPosition().x * 10 - width * 10 / 2;
	hurtbox.y = body->GetPosition().y * 10 - height * 10 / 2;
	
}

void Character::atackStrong(int frameNumber)
{

	//Dependiendo del frame en el que esté, hara una cosa u otra..

	switch (frameNumber)
	{
	case 0:
		//Empieza el ataque :v
		std::cout << "primer frame" << std::endl;
		break;
	case 90:
	{

		//Al frame 90, crea un rect y si el oponente colisiona con ello...
		SDL_Rect hitbox = { 
			(int)(body->GetPosition().x * 10 - width * 2.5f + dir * 50),
			(int)(body->GetPosition().y * 10 - height * 2.5f), 
			(int)width * 5, 
			(int)height * 5 };

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
		std::cout << "acabe we" << std::endl;

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
	case 0:
		std::cout << "primer frame" << std::endl;
		break;
	case 12:
	{
		SDL_Rect hitbox = {
			(int)(body->GetPosition().x * 10 - width * 5 + dir * 30),
			(int)(body->GetPosition().y * 10 - height * 5),
			(int)width * 10,
			(int)height * 10 };

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		if (SDL_HasIntersection(&hitbox, oponent->GetHurtbox()))
		{
			oponent->GetHit(ataqueDebil, dir);
		}
	}
	break;
	case 20:
		std::cout << "acabe we" << std::endl;
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}
void Character::SetGround(bool ground)
{
	onGround = ground;
}

void Character::draw()
{
	//if (debug)
		SDL_SetRenderDrawColor(sdl->renderer(), 0, 0, 255, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hurtbox);

	//dibujar los sprite bruh
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
