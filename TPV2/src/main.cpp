// This file is part of the course TPV2@UCM - Samir Genaim


#include <SDL.h>
#include <iostream>
#include <box2d.h>

#include "sdlutils/InputHandler.h"
#include "sdlutils/macros.h"

#include "sdlutils/sdlutils_demo.h"
#include "sdlutils/SDLUtils.h"

int main(int ac, char **av) {

	//Creamos el espacio fisico
	b2Vec2 gravity = b2Vec2(0.0f, 10.0f);

	b2World world = b2World(gravity);

	//Definimos un objeto (dinámico)
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(200.0f, 0.0f);
	groundBodyDef.type = b2_dynamicBody;

	//Añadimos un objeto con la definicion anterior
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	//Definimos un caja
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(10.0f, 10.0f);

	//Creamos una "cuerpo" 
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0005f;

	//añadimos el cuerpo al objeto fisico
	groundBody->CreateFixture(&fixtureDef);

	groundBody->SetFixedRotation(true);

	//--------------------------

	//Definimos un objeto (estatico)
	b2BodyDef groundDef;
	groundDef.position.Set(250.0f, 300.0f);
	groundDef.type = b2_staticBody;

	//Añadimos al mundo
	b2Body* ground = world.CreateBody(&groundDef);

	//Le damos forma...
	b2PolygonShape floor;
	floor.SetAsBox(250.0f, 100.0f);

	//..cuerpo
	b2FixtureDef fixt;
	fixt.shape = &floor;
	fixt.density = 10.0f;
	fixt.friction = 0.2f;

	ground->CreateFixture(&fixt);

	//---------------------------------------------------------
	
	// Initialise the SDLGame singleton
	SDLUtils::init("SDLGame Demo!", 800, 600,
		"resources/config/resources.json");

	auto& sdl = *SDLUtils::instance();

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
	SDL_Rect scene = { 0, 200, 500, 200 };

	SDL_Rect box = { 100, -10, 20, 20 };


	//Bucle que estaba en la demo pero modificado xd
	while (!exit_) {
		Uint32 startTime = sdl.currRealTime();

		// update the event handler
		ih.refresh();

		// exit when any key is down
		if (ih.isKeyDown(SDLK_d))
		{
			groundBody->ApplyForceToCenter(b2Vec2(10, 0), true);
		}
		else if (ih.isKeyDown(SDLK_a))
		{
			groundBody->ApplyForceToCenter(b2Vec2(-10, 0), true);
		}
		else if (ih.isKeyDown(SDLK_w))
		{
			groundBody->ApplyForceToCenter(b2Vec2(0, -1000), true);
		}
		else if (ih.isKeyDown(SDLK_ESCAPE))
			exit_ = true;

		//Esto llama al mundo para que simule lo que pasa en el tiempo que se le pase (en este caso 1000.f/30.f (un frame a 30 fps))
		world.Step(1000.f/30.f, 10, 16);

		// clear screen
		sdl.clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

		//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
		x2 = groundBody->GetPosition().x;
		y2 = groundBody->GetPosition().y;
		box.x = x2 - 10;
		box.y = y2 - 10;

		//Dibujamos las cajas
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &scene);
		SDL_RenderDrawRect(renderer, &box);

		// present new frame
		sdl.presentRenderer();

		Uint32 frameTime = sdl.currRealTime() - startTime;

		if (frameTime < 1000.f / 30.f)
		{
			SDL_Delay(1000.f / 30.f - frameTime);
		}
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

