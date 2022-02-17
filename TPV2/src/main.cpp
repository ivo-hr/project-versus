// This file is part of the course Proyectos2@UCM - LeftOvers Productions �

#include <SDL.h>
#include <iostream>
#include <box2d.h>

#include "sdlutils/InputHandler.h"
#include "sdlutils/macros.h"

#include "sdlutils/sdlutils_demo.h"
#include "sdlutils/SDLUtils.h"

int main(int ac, char **av) {

	//Creamos el espacio fisico
	b2Vec2 gravity = b2Vec2(0.0f, 20.0f);

	b2World world = b2World(gravity);

	float width = 5.f;
	float height = 5.f;

	//Definimos un objeto (din�mico)
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(50.0f, 70.0f);
	groundBodyDef.type = b2_dynamicBody;

	//A�adimos un objeto con la definicion anterior
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	//Definimos un caja
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width / 2, height / 2);

	//Creamos una "cuerpo" 
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 10.f;
	fixtureDef.friction = 0.9f;

	//a�adimos el cuerpo al objeto fisico
	groundBody->CreateFixture(&fixtureDef);

	groundBody->SetFixedRotation(true);

	groundBody->SetGravityScale(10.f);

	//--------------------------

	//Definimos un objeto (estatico)
	b2BodyDef groundDef;
	groundDef.position.Set(96.0f, 90.0f);
	groundDef.type = b2_staticBody;

	//A�adimos al mundo
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

	//---------------------------------------------------------
	
	// Initialise the SDLGame singleton
	SDLUtils::init("Project Vs21", 900, 800,
		"resources/config/resources.json");

	auto& sdl = *SDLUtils::instance();

	//sdl.toggleFullScreen();

	//show the cursor
	sdl.showCursor();

	// store the 'renderer' in a local variable, just for convenience
	SDL_Renderer* renderer = sdl.renderer();

	// some coordinates
	auto winWidth = sdl.width();
	auto winHeight = sdl.height();
	int32 x2 = 200;
	int32 y2 = 0;

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto& ih = *InputHandler::instance();

	// a boolean to exit the loop
	bool exit_ = false;

	//-----------------------------------------------------

	//Creo las cajas que representaran a los objetos
	SDL_Rect scene = { 960.0f - 750.0f, 900.0f - 50.f, 1500, 10 };

	SDL_Rect box = { (groundBody->GetPosition().x * 10 - width * 10 / 2), (groundBody->GetPosition().y * 10 - height * 10 / 2), width * 10, height * 10 };


	int32 speed = 0;

	//Bucle que estaba en la demo pero modificado xd
	while (!exit_) {
		Uint32 startTime = sdl.currRealTime();

		//groundBody->SetLinearVelocity(b2Vec2(10, 0));

		// update the event handler
		ih.refresh();

		// exit when any key is down
		if (ih.isKeyDown(SDLK_d))
		{
			speed = 40;
		}
		if (ih.isKeyDown(SDLK_a))
		{
			speed = -40;
		}
		if (ih.isKeyDown(SDLK_w))
		{
			groundBody->ApplyLinearImpulseToCenter(b2Vec2(0, -1000 * world.GetGravity().y), true);
		}
		else if (ih.isKeyDown(SDLK_ESCAPE))
			exit_ = true;



		groundBody->SetLinearVelocity(b2Vec2(speed, groundBody->GetLinearVelocity().y));

		//Esto llama al mundo para que simule lo que pasa en el tiempo que se le pase (en este caso 1000.f/30.f (un frame a 30 fps))

		double step = 1.f / 60.f;
		world.Step(step, 1, 1);

		// clear screen
		sdl.clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

		//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
		x2 = groundBody->GetPosition().x * 10;
		y2 = groundBody->GetPosition().y * 10;
		box.x = x2 - width * 10 / 2;
		box.y = y2 - height * 10 / 2;

		//Dibujamos las cajas
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &scene);
		SDL_RenderDrawRect(renderer, &box);

		// present new frame
		sdl.presentRenderer();

		double frameTime = sdl.currRealTime() - startTime;

		if (frameTime < step)
		{
			SDL_Delay(step - frameTime);
		}

		std::cout << groundBody->GetLinearVelocity().x << "\n";
	}

	std::cout << ground->GetPosition().y;

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

