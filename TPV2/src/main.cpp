// This file is part of the course Proyectos2@UCM - LeftOvers Productions ®

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include "game/Character.h"
#include "sdlutils/InputHandler.h"
#include "sdlutils/macros.h"

#include "sdlutils/sdlutils_demo.h"
#include "sdlutils/SDLUtils.h"

bool onGround;

class myListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};
void myListener::BeginContact(b2Contact* contact)
{
	onGround = true;
	b2Body* one = contact->GetFixtureA()->GetBody();
	b2Body* two = contact->GetFixtureB()->GetBody();
	std::cout << "contacto" << std::endl;
}
void myListener::EndContact(b2Contact* contact)
{
	onGround = false;
	std::cout << "fin contacto" << std::endl;
}

int main(int ac, char **av) {

	// Initialise the SDLGame singleton
	SDLUtils::init("Project Vs21", 1024, 576,
		"resources/config/resources.json");

	auto& sdl = *SDLUtils::instance();

	//Obtenemos el tamaño de la pantalla
	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(0, &DM);

	/*
	float scaleX = (float)DM.w / sdl.width();
	float scaleY = (float)DM.h / sdl.height();

	SDL_RenderSetScale(sdl.renderer(), scaleX, scaleY);
	*/

	//Escalamos toda la ventana para que se ajuste al tamaño de la pantalla
	SDL_RenderSetLogicalSize(sdl.renderer(), DM.w, DM.h);

	//Cambiamos el tamaño de la ventana
	SDL_SetWindowSize(sdl.window(), DM.w, DM.h);

	//Ponemos en pantalla completa
	sdl.toggleFullScreen();

	//show the cursor
	sdl.showCursor();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto& ih = *InputHandler::instance();

	//-----------------------------------------------------------------------------------------

	//Creamos el espacio fisico
	b2Vec2 gravity = b2Vec2(0.0f, 20.0f);

	b2World world = b2World(gravity);

	//creamos el detector de colisiones
	myListener listener;
	world.SetContactListener(&listener);


	//Definimos un objeto (dinámico)
	float width = 5.f;
	float height = 5.f;

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

	b2Body* character1 = world.CreateBody(&groundBodyDef);
	//añadimos el cuerpo al objeto fisico
	character1->CreateFixture(&fixtureDef);

	character1->SetFixedRotation(true);

	character1->SetGravityScale(10.f);


	//---------------------------------------------------------


	//Definimos un objeto (estatico)
	b2BodyDef groundDef;
	groundDef.position.Set(96.0f, 90.0f);
	groundDef.type = b2_staticBody;

	//Añadimos al mundo
	b2Body* ground = world.CreateBody(&groundDef);

	//Le damos forma...
	b2PolygonShape floor;
	floor.SetAsBox(75.0f, 5.0f);

	//..cuerpo
	b2FixtureDef fixt;
	fixt.shape = &floor;
	fixt.density = 10.0f;
	fixt.friction = 0.7f;

	ground->CreateFixture(&fixt);

	//--------------------------
	
	//Creo las cajas que representaran a los objetos
	SDL_Rect scene = { 960.0f - 750.0f, 900.0f - 50.f, 1500, 10 };

	SDL_Rect box = { (character1->GetPosition().x * 10 - width * 10 / 2), (character1->GetPosition().y * 10 - height * 10 / 2), width * 10, height * 10 };

	int32 speed = 0;

	// a boolean to exit the loop
	bool exit_ = false;
	int32 x2;
	int32 y2;

	onGround = true;

	int iniJumps = 1;
	int eJumps = iniJumps;
	int moveBuffer=0;
	bool moveing = false;
	//Bucle que estaba en la demo pero modificado xd
	while (!exit_) {
		Uint32 startTime = sdl.currRealTime();

		//groundBody->SetLinearVelocity(b2Vec2(10, 0));
		
		//number of extra jumps
		// update the event handler
		ih.refresh();
		// exit when any key is down
		if (ih.isKeyDown(SDLK_d))
		{
			speed = 40;
			moveing = true;

		}
		if (ih.isKeyDown(SDLK_a))
		{
			speed = -40;
			moveing = true;
		}
		if (ih.isKeyUp(SDLK_a) && ih.isKeyUp(SDLK_d))
		{
			// para que no haya movimiento infinito (experimental)
			moveing = false;
		}
		if (ih.isKeyDown(SDLK_w)&& eJumps>0)
		{
			if (!onGround)
			{
				eJumps--;
			}
			
			character1->ApplyLinearImpulseToCenter(b2Vec2(0, -1000 * world.GetGravity().y), true);
		}
		else if (ih.isKeyDown(SDLK_ESCAPE))
				exit_ = true;
		//boolean to check collision with the ground
		if (onGround)
		{
			eJumps = iniJumps;
		}

		if (!moveing)
		{
			if(speed > 0)
				speed--;
			if (speed < 0)
				speed++;
		}

		character1->SetLinearVelocity(b2Vec2(speed, character1->GetLinearVelocity().y));

		//Esto llama al mundo para que simule lo que pasa en el tiempo que se le pase (en este caso 1000.f/30.f (un frame a 30 fps))

		double step = 1.f / 60.f;
		world.Step(step, 1, 1);

		// clear screen
		sdl.clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

		//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
		x2 = character1->GetPosition().x * 10;
		y2 = character1->GetPosition().y * 10;
		box.x = x2 - width * 10 / 2;
		box.y = y2 - height * 10 / 2;

		//Dibujamos las cajas
		SDL_SetRenderDrawColor(sdl.renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl.renderer(), &scene);
		SDL_RenderDrawRect(sdl.renderer(), &box);

		// present new frame
		sdl.presentRenderer();

		double frameTime = sdl.currRealTime() - startTime;

		if (frameTime < step)
		{
			SDL_Delay(step - frameTime);
		}
	}

	//std::cout << "Hello World!\n";
	//try {
	//	sdlutils_basic_demo();
	//} catch (const std::string &e) {		// catch exceptions thrown as strings
	//	std::cerr << e << std::endl;
	//} catch (const char *e) {				// catch exceptions thrown as char*
	//	std::cerr << e << std::endl;
	//} catch (const std::exception &e) {		// catch exceptions thrown as a sub-type of std::exception
	//	std::cerr << e.what();
	//} catch (...) {
	//	std::cerr << "Caught and exception of unknown type ...";
	//}


	return 0;
}
