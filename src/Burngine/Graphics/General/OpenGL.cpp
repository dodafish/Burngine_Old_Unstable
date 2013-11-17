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

namespace burn {

//Static member declaration
std::vector<GLFWwindow*> ContextHandler::_windows;
bool ContextHandler::_contextEnsured = false;
bool ContextHandler::_isGlewInitialized = false;
bool ContextHandler::_isGlfwInitialized = false;
GLFWwindow* ContextHandler::_fakeWindow = nullptr;

void ContextHandler::registerWindow(GLFWwindow* window,
									bool activateContext) {

	//window must not be a nullptr
	if(window == nullptr){
		Reporter::report("Failed to register window! Nullptr exception!", Reporter::ERROR);
		return;
	}

	//window must not be registered already
	for(size_t i = 0; i < _windows.size(); ++i){
		if(_windows[i] == window){
			//Even though it's no fatal error, it must not happen.
			Reporter::report("Failed to register window! Window is already registered!", Reporter::ERROR);
			return;
		}
	}

	//Ok, window is alright. Register it and activate its context if wanted
	_windows.push_back(window);
	if(activateContext){
		glfwMakeContextCurrent(window);
		_contextEnsured = true; //We now have a context
	}

	//Window successfully registered :)
}

void ContextHandler::deregisterWindow(GLFWwindow* window) {

	//window must not be a nullptr
	if(window == nullptr){
		Reporter::report("Failed to deregister window! Nullptr exception!", Reporter::ERROR);
		return;
	}

	//window must be registered already
	bool isRegistered = false;
	for(size_t i = 0; i < _windows.size(); ++i){
		if(_windows[i] == window){
			isRegistered = true;
			break;
		}
	}
	if(!isRegistered){
		Reporter::report("Failed to deregister window! Window was not registered before!", Reporter::ERROR);
		return;
	}

	//Remove window from list
	for(size_t i = 0; i < _windows.size(); ++i){
		if(_windows[i] == window){
			_windows.erase(_windows.begin() + i);
			break;
		}
	}

	//Is there a need for a valid context?
	if(_contextEnsured){
		//We have to ensure a context. So deregistering a window with an activated context is fatal

		//Is window the current context?
		if(window == glfwGetCurrentContext()){
			_contextEnsured = false;
			ensureContext(); //Choose another context (or fake one)
		}
	}

	//Window successfully deregistered :)
}

void ContextHandler::ensureContext() {

	//No need to find or make a context?
	if(_contextEnsured){
		return;
	}

	//The following commands need GLFW
	ensureGlfw();

	//Try to activate a window's context
	if(_windows.size() != 0){
		glfwMakeContextCurrent(_windows[0]); //Choose the first window in the list
	}else{

		//We have to fake a window for having a valid context
		if(_fakeWindow != nullptr){

			//We already have a faked window. Use it
			glfwMakeContextCurrent(_fakeWindow);

		}
		else{

			//We have to create a fakewindow first
			glfwDefaultWindowHints(); //Reset window hints
			glfwWindowHint(GLFW_VISIBLE, GL_FALSE); //Hide the window
			_fakeWindow = glfwCreateWindow(1, 1, "", 0, 0);
			glfwDefaultWindowHints(); //Reset window hints

			//Check if creation succeeded
			if(_fakeWindow == nullptr){
				Reporter::report("Failed to ensure context! Creation of fake window failed!", Reporter::ERROR);
				exit(1); //Terminate program... :(
			}

			//Fake window is created. Make its context current
			glfwMakeContextCurrent(_fakeWindow);

		}

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
		return;
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
		return;
	}

	_isGlfwInitialized = true;
}

}
