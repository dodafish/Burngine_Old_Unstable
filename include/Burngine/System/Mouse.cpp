/*
 * Mouse.cpp
 *
 *  Created on: 05.10.2013
 *      Author: Dominik
 */

#include "Mouse.h"

namespace burn {

bool Mouse::_buttons[];
Vector2d Mouse::_cursorPosition;

void Mouse::buttonCallback(GLFWwindow * window, int button, int action, int mods) {
	if(action == GLFW_PRESS){
		_buttons[button] = true;
	}else if(action == GLFW_RELEASE){
		_buttons[button] = false;
	}
}

void Mouse::cursorPosCallback(GLFWwindow * window, double x, double y) {
	_cursorPosition.x = x;
	_cursorPosition.y = y;
}

bool Mouse::isButtonPressed(Mouse::Button button) {
	return _buttons[button];
}

const Vector2d& Mouse::getCursorPosition() {
	return _cursorPosition;
}

void Mouse::setCursorPosition(const Window& relativeWindow, const Vector2d& position) {
	_cursorPosition = position;
	relativeWindow.setCursorPosition(position);
}

} /* namespace burn */
