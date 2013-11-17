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

#include <Burngine/Graphics/General/OpenGL.h>

#include <Burngine/System/Reporter.h>

//For the exit() func
#include <cstdlib>

namespace burn {

//Static member declaration
bool ContextHandler::_contextEnsured = false;
bool ContextHandler::_isGlewInitialized = false;
bool ContextHandler::_isGlfwInitialized = false;
GLFWwindow* ContextHandler::_fakeWindow = nullptr;
GLFWwindow* ContextHandler::_preferredWindow = nullptr;
GLFWwindow* ContextHandler::_currentContextWindow = nullptr;
std::vector<GLFWwindow*> ContextHandler::_windows;

void ContextHandler::shutdown() {

	//This destroys all windows automatically
	glfwTerminate();

	_windows.clear();
	_fakeWindow = nullptr;
	_preferredWindow = nullptr;
	_currentContextWindow = nullptr;
	_isGlewInitialized = false;
	_isGlfwInitialized = false;
	_contextEnsured = false;

}

void ContextHandler::useContext(GLFWwindow* window) {

	if(_currentContextWindow == window){
		//Context already in use
		return;
	}

	_preferredWindow = window;
	_contextEnsured = false;

	ensureContext();
}

GLFWwindow* ContextHandler::createWindow(const WindowSettings& settings) {

	//We need GLFW here
	ensureGlfw();

	//The window to return and store
	GLFWwindow* window;

	//Create OpenGL window
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //No resizable window
	window = glfwCreateWindow(	static_cast<int>(settings.getWidth()),
								static_cast<int>(settings.getHeight()),
								settings.getTitle().c_str(),
								settings.isFullscreen() ? glfwGetPrimaryMonitor() : 0, //Pass the primary monitor if we want fullscreen
								_fakeWindow != nullptr ? _fakeWindow : 0);

	//Check if creation succeeded
	if(window == nullptr){
		Reporter::report("Failed to create window!", Reporter::ERROR);
		glfwTerminate();
		exit(4);
	}else{
		//Store window in list
		_windows.push_back(window);
		_preferredWindow = window;
		_contextEnsured = false; //Use the new window, not the old context
	}

	ensureContext();

	return window;
}

void ContextHandler::destroyWindow(GLFWwindow* window) {

	if(!_isGlfwInitialized){
		//If there is no GLFW initialized, there can't be any window
		_windows.clear();
		return;
	}

	//First remove it from the list
	for(size_t i = 0; i < _windows.size(); ++i){
		if(_windows[i] == window){
			_windows.erase(_windows.begin() + i);
			break;
		}
	}

	//Remove it from preferred when necessary
	if(window == _preferredWindow){
		_preferredWindow = nullptr;
		_contextEnsured = false; //Find a new one, as the old one might be the destroyed window
	}

	//Finally destroy the window
	glfwDestroyWindow(window);

	//Done :)
}

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
			glfwDefaultWindowHints(); //Reset window hints
			glfwWindowHint(GLFW_VISIBLE, GL_FALSE); //Hide the window
			_fakeWindow = glfwCreateWindow(1, 1, "", 0, 0);
			glfwDefaultWindowHints(); //Reset window hints

			//Check if creation succeeded
			if(_fakeWindow == nullptr){
				Reporter::report("Failed to ensure context! Creation of fake window failed!", Reporter::ERROR);
				glfwTerminate(); // <- ensureGlfw initialized it. Terminate before end
				exit(1); //Terminate program... :(
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
		Reporter::report("Failed to initialize GLEW!", Reporter::ERROR);
		glfwTerminate();
		exit(2);
	}

	if(GLEW_VERSION_4_3){
		Reporter::report("OpenGL 4.3 supported");
	}else if(GLEW_VERSION_4_2){
		Reporter::report("OpenGL 4.2 supported");
	}else if(GLEW_VERSION_4_1){
		Reporter::report("OpenGL 4.1 supported");
	}else if(GLEW_VERSION_4_0){
		Reporter::report("OpenGL 4.0 supported");
	}else if(GLEW_VERSION_3_3){
		Reporter::report("OpenGL 3.3 supported");
	}else{
		Reporter::report(	"OpenGL 3.3 or higher is not supported! Try updating your videocard's driver.",
							Reporter::ERROR);
		glfwTerminate();
		exit(5);
	}

	_isGlewInitialized = true;
	_contextEnsured = true;
}

void ContextHandler::ensureGlfw() {

	//Is GLFW already initialized?
	if(_isGlfwInitialized)
		return;

	//Initialize GLFW
	if(!glfwInit()){
		Reporter::report("Failed to initialize GLFW!", Reporter::ERROR);
		exit(3);
	}

	_isGlfwInitialized = true;
}

}
