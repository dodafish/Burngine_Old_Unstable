//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#include <Burngine/System/Mouse.h>

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
