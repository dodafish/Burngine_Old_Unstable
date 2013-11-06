//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

#ifndef MOUSE_H_
#define MOUSE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h> ///< GLFW
#include <Burngine/Graphics/Window/Window.h> ///< for setting cursor position
#include <Burngine/System/Math.h>
namespace burn {

class BURNGINE_API Mouse {
public:
	Mouse() = delete;

	enum Button {
		LEFT = 0, RIGHT, MIDDLE, _4, _5, _6, _7, _8
	};

	static void buttonCallback(GLFWwindow * window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow * window, double x, double y);

	static bool isButtonPressed(Button button);
	static const Vector2d& getCursorPosition();

	static void setCursorPosition(const Window& relativeWindow, const Vector2d& position);

private:
	static bool _buttons[GLFW_MOUSE_BUTTON_LAST + 1]; ///< status of mousebuttons
	static Vector2d _cursorPosition;
};

} /* namespace burn */
#endif /* MOUSE_H_ */
