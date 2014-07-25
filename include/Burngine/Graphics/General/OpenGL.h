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
#include <Burngine/System/Log.h>

//For the exit() func
#include <cstdlib>

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
	inline static void ensureContext();

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

void ContextHandler::ensureContext() {

	//No need to find or make a context?
	if(_contextEnsured){
		return;
	}

	//Try using the context of a real window
	if(_isGlfwInitialized && _windows.size() != 0){
		if(_preferredWindow != nullptr){
			//Use the context of the preferred window
			glfwMakeContextCurrent(_preferredWindow);
			_currentContextWindow = _preferredWindow;
		}else{
			//Just use the first window of the list
			glfwMakeContextCurrent(_windows[0]);
			_currentContextWindow = _windows[0];
		}
	}else{

		//When we land here we either have no windows or GLFW is not initialized.
		//If the latter is the case, there can't be any window at all, so clear
		//the vector just to be sure...
		_windows.clear();

		//The following commands need GLFW
		ensureGlfw();

		//We have to fake a window for having a valid context
		if(_fakeWindow != nullptr){
			//We already have a faked window. Use it
			glfwMakeContextCurrent(_fakeWindow);
		}else{
			//We have to create a fakewindow first
			glfwDefaultWindowHints();    //Reset window hints
			glfwWindowHint(GLFW_VISIBLE, GL_FALSE);    //Hide the window
			_fakeWindow = glfwCreateWindow(1, 1, "", 0, 0);
			glfwDefaultWindowHints();    //Reset window hints

			//Check if creation succeeded
			if(_fakeWindow == nullptr){
				Log::log("Failed to ensure context! Creation of fake window failed!", Log::ERROR);
				glfwTerminate();    // <- ensureGlfw initialized it. Terminate before end
				exit(1);    //Terminate program... :(
			}

			//Fake window is created. Make its context current
			glfwMakeContextCurrent(_fakeWindow);
		}
		_currentContextWindow = _fakeWindow;

	}

	////////////////////////////////////////////////////////
	// Ensure that we have GLEW initialized
	////////////////////////////////////////////////////////

	//Is GLEW initialized already?
	if(_isGlewInitialized){
		_contextEnsured = true;
		return;
	}

	//GLEW needs a valid context for creation. We have ensured this above.

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		Log::log("Failed to initialize GLEW!", Log::ERROR);
		glfwTerminate();
		exit(2);
	}

	if(GLEW_VERSION_4_3){
		Log::log("OpenGL 4.3 supported");
	}else if(GLEW_VERSION_4_2){
		Log::log("OpenGL 4.2 supported");
	}else if(GLEW_VERSION_4_1){
		Log::log("OpenGL 4.1 supported");
	}else if(GLEW_VERSION_4_0){
		Log::log("OpenGL 4.0 supported");
	}else if(GLEW_VERSION_3_3){
		Log::log("OpenGL 3.3 supported");
	}else{
		Log::log(	"OpenGL 3.3 or higher is not supported! Try updating your videocard's driver.",
							Log::ERROR);
		glfwTerminate();
		exit(5);
	}

	_isGlewInitialized = true;
	_contextEnsured = true;
}

/*
 * @brief Just a shortcut for ContextHandler::ensureContext()
 */
inline void ensureContext() {
	ContextHandler::ensureContext();
}

}

#endif /* OPENGL_H_ */
