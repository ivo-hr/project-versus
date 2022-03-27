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
			whichOne = event.jaxis.which;
			m_buttonStates[whichOne][event.jbutton.button] = true;
			break;
		case SDL_JOYBUTTONUP:
			whichOne = event.jaxis.which;
			m_buttonStates[whichOne][event.jbutton.button] = false;
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
			update(event);
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
	bool m_bJoysticksInitialised;

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
					//m_buttonStates.push_back(tempButtons);
					m_buttonStates[SDL_JoystickInstanceID(joy)] = tempButtons;
				}
				else
				{
					std::cout << SDL_GetError();
				}
			}
			SDL_JoystickEventState(SDL_ENABLE);
			m_bJoysticksInitialised = true;
		}
		else
		{
			m_bJoysticksInitialised = false;
		}
	}

	std::vector<std::vector<bool>> m_buttonStates;
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
