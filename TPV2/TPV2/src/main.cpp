// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <box2d.h>

#include "sdlutils/sdlutils_demo.h"

int main(int ac, char **av) {

	b2Vec2 gravity = b2Vec2(0.0f, -10.0f);

	b2World world = b2World(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);


	std::cout << "Hello World!\n";
	try {
		sdlutils_basic_demo();
	} catch (const std::string &e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	} catch (const char *e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	} catch (const std::exception &e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	} catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	return 0;
}

