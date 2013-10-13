/*
 * Mouse.h
 *
 *  Created on: 05.10.2013
 *      Author: Dominik
 */

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
