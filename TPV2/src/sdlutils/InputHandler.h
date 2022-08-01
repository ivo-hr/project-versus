// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <iostream>
#include <SDL.h>
#include <array>

#include "../utils/Singleton.h"
#include <vector>

// Instead of a Singleton class, we could make it part of
// SDLUtils as well.

class InputHandler: public Singleton<InputHandler> {

	friend Singleton<InputHandler> ;

public:
	enum MOUSEBUTTON : uint8_t {
		LEFT = 0, MIDDLE = 1, RIGHT = 2
	};

	virtual ~InputHandler() {
	}

	// clear the state
	inline void clearState() {
		isKeyDownEvent_ = false;
		isKeyUpEvent_ = false;
		isMouseButtonEvent_ = false;
		isMouseMotionEvent_ = false;
		for (auto i = 0u; i < 3; i++) {
			mbState_[i] = false;
		}
	}

	// update the state with a new event
	inline void update(const SDL_Event &event) {
		int whichOne;
		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonChange(event, true);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonChange(event, false);
			break;
		case  SDL_JOYBUTTONDOWN:
	/*		whichOne = event.jaxis.which;
			m_buttonStates[whichOne][event.jbutton.button] = true;*/
			break;
		case SDL_JOYBUTTONUP:
	/*		whichOne = event.jaxis.which;
			m_buttonStates[whichOne][event.jbutton.button] = false;*/
			break;
		default:
			break;
		}
	}

	// refresh
	inline void refresh() {
		SDL_Event event;

		clearState();
		while (SDL_PollEvent(&event))
		{
			update(event);
		}
	}

	// keyboard
	inline bool keyDownEvent() {
		return isKeyDownEvent_;
	}

	inline bool keyUpEvent() {
		return isKeyUpEvent_;
	}

	inline bool isKeyDown(SDL_Scancode key) {
		return kbState_[key] == 1;
	}

	inline bool isKeyDown(SDL_Keycode key) {
		return isKeyDown(SDL_GetScancodeFromKey(key));
	}

	inline bool isKeyUp(SDL_Scancode key) {
		return kbState_[key] == 0;
	}

	inline bool isKeyUp(SDL_Keycode key) {
		return isKeyUp(SDL_GetScancodeFromKey(key));
	}

	// mouse
	inline bool mouseMotionEvent() {
		return isMouseMotionEvent_;
	}

	inline bool mouseButtonEvent() {
		return isMouseButtonEvent_;
	}

	inline const std::pair<Sint32, Sint32>& getMousePos() {
		return mousePos_;
	}

	inline int getMouseButtonState(MOUSEBUTTON b) {
		return mbState_[b];
	}

	// NES controller

	inline void initialiseJoysticks() {
		if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
		{
			SDL_InitSubSystem(SDL_INIT_JOYSTICK);
		}
		if (SDL_NumJoysticks() > 0)
		{
			m_buttonStates.resize(SDL_NumJoysticks());
			for (int i = 0; i < SDL_NumJoysticks(); i++)
			{
				SDL_Joystick* joy = SDL_JoystickOpen(i);

				if (joy)
				{
					std::vector<bool> tempButtons;
					for (int j = 0; j < SDL_JoystickNumButtons(joy); j++)
					{
						tempButtons.push_back(false);
					}
					m_buttonStates[SDL_JoystickInstanceID(joy)] = tempButtons;
				}
				else
				{
					std::cout << SDL_GetError();
				}
			}
			SDL_JoystickEventState(SDL_ENABLE);
		}
	}

	std::vector<std::vector<bool>> m_buttonStates;
	// buttonNumber: 1 = A / 2 = B (NES)
	inline bool getButtonState(int joy, int buttonNumber)
	{
		if (SDL_NumJoysticks() > joy) return m_buttonStates[joy][buttonNumber];
		return false;
	}

	inline int getAxesState(int joy, int axesNumber) {
		if (SDL_NumJoysticks() > joy) {
			SDL_JoystickID id = joy;
			SDL_Joystick* joystick = SDL_JoystickFromInstanceID(id);
			int value;
			if (SDL_JoystickGetAxis(joystick, axesNumber) > -100)value = 1;
			else if (SDL_JoystickGetAxis(joystick, axesNumber) < -300)value = -1;
			else value = 0;
			return value;
		}
		else return 0;
	}

	// direction: 0=up / 1=right / 2=down / 3=left
	inline bool xboxGetDpadState(int joy, int direction) {
		if (SDL_NumJoysticks() > joy) {
			SDL_GameController* gc = SDL_GameControllerOpen(SDL_NumJoysticks() - joy - 1);
			switch (direction)
			{
			case 0:
				return SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_UP);
				break;
			case 1:
				return SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
				break;
			case 2:
				return SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
				break;
			case 3:
				return SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
				break;
			default:
				return false;
				break;
			}
		}
		else return false;
	}

	// axesNumber: 0=joystick izq eje X / 1=joystick izq eje Y / 2=L2 trigger / 3=joystick der eje X
	//	4=joystick der eje Y / 5=R2 trigger
	inline int xboxGetAxesState(int joy, int axesNumber) {
		if (SDL_NumJoysticks() > joy) {
			SDL_GameController* gc = SDL_GameControllerOpen(SDL_NumJoysticks() - joy - 1);
			SDL_JoystickID id = joy;
			SDL_Joystick* joystick = SDL_JoystickOpen(SDL_NumJoysticks() - joy - 1);
			int deathZoneMin, deathZoneMax;
			
			if (SDL_JoystickNumAxes(joystick) < 6) { // if NES controller
				deathZoneMin = -300;
				deathZoneMax = -100;
				if (axesNumber == 0)axesNumber = 1;
				else if (axesNumber == 1)axesNumber = 4;
				if (SDL_JoystickGetAxis(joystick, axesNumber) > deathZoneMax)return 1;
				else if (SDL_JoystickGetAxis(joystick, axesNumber) < deathZoneMin)return -1;
				else return 0;
			}
			else { // if any other controller
				SDL_GameControllerAxis s;
				switch (axesNumber)
				{
				case 0:
					s = SDL_CONTROLLER_AXIS_LEFTX;
					break;
				case 1:
					s = SDL_CONTROLLER_AXIS_LEFTY;
					break;
				}
				deathZoneMax = 15000;
				deathZoneMin = -deathZoneMax;
				if (SDL_GameControllerGetAxis(gc, s) > deathZoneMax)return 1;
				else if (SDL_GameControllerGetAxis(gc, s) < deathZoneMin)return -1;
				else return 0;
			}
		}
		else return 0;
	}

	inline bool xboxGetButtonState(int joy, SDL_GameControllerButton button) {
		if (SDL_NumJoysticks() > joy) {
			SDL_GameController* gc = SDL_GameControllerOpen(SDL_NumJoysticks() - joy - 1);
			return SDL_GameControllerGetButton(gc, button);
		}
		else return false;
	}

	// Para hacer pruebas con los mandos
	inline void controllerAxesTest(int joy, int axesNumber) {
		if (SDL_NumJoysticks() > joy) {
			SDL_JoystickID id = joy;
			SDL_Joystick* joystick = SDL_JoystickFromInstanceID(id);
			std::cout << "eje"<< axesNumber<<": " << SDL_JoystickGetAxis(joystick, axesNumber) << std::endl;
			
			/*SDL_GameController* gc = SDL_GameControllerOpen(0);
			if (SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_UP))std::cout << "Pulsado" << std::endl;*/
			//std::cout << "dpad" << axesNumber << ": " << SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_DPAD_UP) << std::endl;
		}
	}

	/*inline void cleanJoysticks() {
		if (m_bJoysticksInitialised)
		{
			for (unsigned int i = 0; i < SDL_NumJoysticks(); i++)
			{
				SDL_JoystickClose(m_joysticks[i]);
			}
		}
	}*/

	// TODO add support for Joystick, see Chapter 4 of
	// the book 'SDL Game Development'

private:
	InputHandler() {
		kbState_ = SDL_GetKeyboardState(0);
		clearState();
	}

	inline void onKeyDown(const SDL_Event&) {
		isKeyDownEvent_ = true;
	}

	inline void onKeyUp(const SDL_Event&) {
		isKeyUpEvent_ = true;
	}

	inline void onMouseMotion(const SDL_Event &event) {
		isMouseMotionEvent_ = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;

	}

	inline void onMouseButtonChange(const SDL_Event &event, bool isDown) {
		isMouseButtonEvent_ = true;
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mbState_[LEFT] = isDown;
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[RIGHT] = isDown;
			break;
		default:
			break;
		}
	}

	bool isKeyUpEvent_;
	bool isKeyDownEvent_;
	bool isMouseMotionEvent_;
	bool isMouseButtonEvent_;
	std::pair<Sint32, Sint32> mousePos_;
	std::array<bool, 3> mbState_;
	const Uint8 *kbState_;
};

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
inline InputHandler& ih() {
	return *InputHandler::instance();
}
