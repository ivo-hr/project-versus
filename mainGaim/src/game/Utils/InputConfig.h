#pragma once
using namespace std;
#include <string>
#include "../../sdlutils/InputHandler.h"

class InputConfig {
	InputHandler& ih = *InputHandler::instance();
	char typeofInput = 4;

public:
	InputConfig(char c) {
		 typeofInput = c;
	};
	~InputConfig()=default;

	//char seeinput();
	bool up();
	bool down();
	bool left();
	bool right();
	bool basic();
	bool special();
	bool stop();
	void refresh() {
		ih.refresh();
	};

};