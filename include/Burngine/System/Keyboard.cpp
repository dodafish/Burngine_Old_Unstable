/*
 * Keyboard.cpp
 *
 *  Created on: 05.10.2013
 *      Author: Dominik
 */

#include "Keyboard.h"

namespace burn {

bool Keyboard::_keys[];

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS){
		_keys[key] = true;
	}else if(action == GLFW_RELEASE){
		_keys[key] = false;
	}
}

bool Keyboard::isKeyPressed(Keyboard::Key key) {
	return _keys[key];
}

} /* namespace burn */
