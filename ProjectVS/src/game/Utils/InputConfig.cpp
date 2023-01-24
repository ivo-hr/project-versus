#include "InputConfig.h"
#include "../../utils/CheckML.h"

bool InputConfig::up()
{
	if (typeofInput < 0) {
		return ih.isKeyDown(playerPrefs.KeyboardUp(typeofInput == -1));
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 1) == -1 || ih.xboxGetDpadState(typeofInput, 0);
	}
}

bool InputConfig::down()
{
	if (typeofInput < 0) {
		return ih.isKeyDown(playerPrefs.KeyboardDown(typeofInput == -1));
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 1) == 1 || ih.xboxGetDpadState(typeofInput, 2);
	}
}

bool InputConfig::downReleased()
{
	if (typeofInput < 0) {
		return ih.isKeyUp(playerPrefs.KeyboardDown(typeofInput == -1));
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 1) == 0 && !ih.xboxGetDpadState(typeofInput, 2);
	}
}

bool InputConfig::left()
{
	if (typeofInput < 0) {
		return ih.isKeyDown(playerPrefs.KeyboardLeft(typeofInput == -1));
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 0) == -1 || ih.xboxGetDpadState(typeofInput, 3);
	}
	
	
}

bool InputConfig::right()
{
	if (typeofInput < 0) {
		return ih.isKeyDown(playerPrefs.KeyboardRight(typeofInput == -1));
	}
	else {
		return ih.xboxGetAxesState(typeofInput, 0) == 1 || ih.xboxGetDpadState(typeofInput, 1);
	}
}

bool InputConfig::basic()
{
	if (typeofInput < 0) {
		return ih.isKeyDown(playerPrefs.KeyboardBasic(typeofInput == -1));
	}
	else {
		return ih.xboxGetButtonState(typeofInput, playerPrefs.ControllerBasic());
	}
}

bool InputConfig::special()
{
	if (typeofInput < 0) {
		return ih.isKeyDown(playerPrefs.KeyboardSpecial(typeofInput == -1));
	}
	else {
		return ih.xboxGetButtonState(typeofInput, playerPrefs.ControllerSpecial());
	}
}

bool InputConfig::stop()
{
	if (typeofInput < 0) {
		return ih.isKeyUp(playerPrefs.KeyboardRight(typeofInput == -1)) && ih.isKeyUp(playerPrefs.KeyboardLeft(typeofInput == -1));
	}
	else {
		return  ih.xboxGetAxesState(typeofInput, 0) == 0 && !ih.xboxGetDpadState(typeofInput, 1) && !ih.xboxGetDpadState(typeofInput, 3);
	}
}

bool InputConfig::taunt()
{
	if (typeofInput < 0) {
		return ih.isKeyDown(playerPrefs.KeyboardTaunt(typeofInput == -1));
	}
	else {
		return ih.xboxGetButtonState(typeofInput, playerPrefs.ControllerTaunt());
	}
}
