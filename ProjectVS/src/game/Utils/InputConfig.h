#pragma once
using namespace std;
#include <string>
#include "../../sdlutils/InputHandler.h"
#include "PlayerConfigs.h"

class Character;

class InputConfig {
	PlayerConfigs& playerPrefs = *PlayerConfigs::instance();
	InputHandler& ih = *InputHandler::instance();
	char typeofInput = 4;

	Character* owner = nullptr;

public:
	InputConfig(char c, Character* from) {
		 typeofInput = c;
		 owner = from;
	};
	virtual ~InputConfig() {};

	//char seeinput();
	bool up();
	bool down();
	bool downReleased();
	bool left();
	bool right();
	bool basic();
	bool special();
	bool stop();
	bool taunt();
	void refresh() {
		ih.refresh();
	};

	void rumbleController(unsigned short strengh, unsigned short duration);

	Character* OriginalOwner() { return owner; };
	void SetOriginalOwner(Character* a) { owner = a; };

	// Metodo para hacer pruebas con los mandos
	// Se llama en update de character
	//void controllerTest() {
	//	// Util para saber que buttonNumber es cada boton del mando
	//	for (int i = 0; i < 10; i++)if (ih.getButtonState(0, i))std::cout << "Button pressed: " << i << std::endl;

	//	// Para los ejes
	//	ih.controllerAxesTest(0, 0);
	//}

};