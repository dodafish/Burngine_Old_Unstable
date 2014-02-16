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

#ifndef OPENGL_H_
#define OPENGL_H_

#include <Burngine/extern/SOIL.h>
#include <Burngine/extern/GL/glew.h>
#include <Burngine/extern/glfw3.h>

#include <Burngine/Export.h>
#include <Burngine/Graphics/Window/WindowSettings.h>
#include <vector>

namespace burn {

class BURNGINE_API ContextHandler {
public:

	/**
	 * @brief Ensures that any OpenGL context is active.
	 * This allows OpenGL usage even without a "real" window
	 * (There is a hidden fake window otherwise, just for a context)
	 *
	 * @note If previously used, the window which was set with @ref useContext()
	 * before will be used
	 *
	 * @see useContext()
	 */
	static void ensureContext();

	static GLFWwindow* createWindow(const WindowSettings& settings);
	static void destroyWindow(GLFWwindow* window);

	/**
	 * @brief NEVER call this function yourself. Burngine uses it at
	 * program end for cleaning up the OpenGL stuff
	 */
	static void shutdown();

	/**
	 * @brief Sets a window as preferred context and uses its context
	 * simultanously
	 *
	 * @note Further calls to @ref ensureContext() will activate this
	 * window's context
	 *
	 * @see ensureContext()
	 */
	static void useContext(GLFWwindow* window);

	static WindowSettings estimateWindowResolution(const WindowSettings& ws);

private:

	//Static only
	//C-Tor as private (also older C++) as alternative to deleting it (only C++11)
	ContextHandler();

	//-------------------------

	static void ensureGlfw();

	static GLFWwindow* _currentContextWindow;

	static GLFWwindow* _preferredWindow;
	static GLFWwindow* _fakeWindow;
	static std::vector<GLFWwindow*> _windows;    ///< These are real ones
	static bool _contextEnsured;
	static bool _isGlewInitialized, _isGlfwInitialized;
};

/*
 * @brief Just a shortcut for ContextHandler::ensureContext()
 */
inline void ensureContext() {
	ContextHandler::ensureContext();
}

}

#endif /* OPENGL_H_ */
