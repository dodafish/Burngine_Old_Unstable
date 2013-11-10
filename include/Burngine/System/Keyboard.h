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

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h> ///<- GLFW
namespace burn {

/**
 * @brief Handles keyboard input
 */
class BURNGINE_API Keyboard {
public:

	enum Key {
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		F13 = GLFW_KEY_F13,
		F14 = GLFW_KEY_F14,
		F15 = GLFW_KEY_F15,
		F16 = GLFW_KEY_F16,
		F17 = GLFW_KEY_F17,
		F18 = GLFW_KEY_F18,
		F19 = GLFW_KEY_F19,
		F20 = GLFW_KEY_F20,
		F21 = GLFW_KEY_F21,
		F22 = GLFW_KEY_F22,
		F23 = GLFW_KEY_F23,
		F24 = GLFW_KEY_F24,
		F25 = GLFW_KEY_F25,
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,
		_1 = GLFW_KEY_1,
		_2 = GLFW_KEY_2,
		_3 = GLFW_KEY_3,
		_4 = GLFW_KEY_4,
		_5 = GLFW_KEY_5,
		_6 = GLFW_KEY_6,
		_7 = GLFW_KEY_7,
		_8 = GLFW_KEY_8,
		_9 = GLFW_KEY_9,
		_0 = GLFW_KEY_0,
		SPACE = GLFW_KEY_SPACE,
		COMMA = GLFW_KEY_COMMA,
		PERIOD = GLFW_KEY_PERIOD,
		SEMICOLON = GLFW_KEY_SEMICOLON,
		SLASH = GLFW_KEY_SLASH,
		BACKSLASH = GLFW_KEY_BACKSLASH,
		MINUS = GLFW_KEY_MINUS,
		APOSTROPHE = GLFW_KEY_APOSTROPHE,
		EQUAL = GLFW_KEY_EQUAL,
		LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
		RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
		GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
		NUMPAD_1 = GLFW_KEY_KP_1,
		NUMPAD_2 = GLFW_KEY_KP_2,
		NUMPAD_3 = GLFW_KEY_KP_3,
		NUMPAD_4 = GLFW_KEY_KP_4,
		NUMPAD_5 = GLFW_KEY_KP_5,
		NUMPAD_6 = GLFW_KEY_KP_6,
		NUMPAD_7 = GLFW_KEY_KP_7,
		NUMPAD_8 = GLFW_KEY_KP_8,
		NUMPAD_9 = GLFW_KEY_KP_9,
		NUMPAD_0 = GLFW_KEY_KP_0,
		NUMPAD_ADD = GLFW_KEY_KP_ADD,
		NUMPAD_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
		NUMPAD_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
		NUMPAD_DIVIDE = GLFW_KEY_KP_DIVIDE,
		NUMPAD_ENTER = GLFW_KEY_KP_ENTER,
		NUMPAD_EQUAL = GLFW_KEY_KP_EQUAL,
		NUMPAD_DECIMAL = GLFW_KEY_KP_DECIMAL,
		ESCAPE = GLFW_KEY_ESCAPE,
		ENTER = GLFW_KEY_ENTER,
		TAB = GLFW_KEY_TAB,
		BACKSPACE = GLFW_KEY_BACKSPACE,
		INSERT = GLFW_KEY_INSERT,
		DELETE = GLFW_KEY_DELETE,
		UP = GLFW_KEY_UP,
		DOWN = GLFW_KEY_DOWN,
		LEFT = GLFW_KEY_LEFT,
		RIGHT = GLFW_KEY_RIGHT,
		PAGE_UP = GLFW_KEY_PAGE_UP,
		PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
		HOME = GLFW_KEY_HOME,
		END = GLFW_KEY_END,
		CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
		SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
		NUM_LOCK = GLFW_KEY_NUM_LOCK,
		PRINT = GLFW_KEY_PRINT_SCREEN,
		PAUSE = GLFW_KEY_PAUSE,
		LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
		RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
		LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
		RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
		LEFT_ALT = GLFW_KEY_LEFT_ALT,
		RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
		LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
		RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
		MENU = GLFW_KEY_MENU
	};

	Keyboard() = delete; //static only

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/**
	 * @brief Checks if a key is being pressed or not
	 *
	 * @param key The key to check
	 *
	 * @return Returns true if the key is being pressed. False otherwise
	 */
	static bool isKeyPressed(Key key);

private:

	static bool _keys[GLFW_KEY_LAST + 1]; ///< Saves pressed status of keys

};

} /* namespace burn */
#endif /* KEYBOARD_H_ */
