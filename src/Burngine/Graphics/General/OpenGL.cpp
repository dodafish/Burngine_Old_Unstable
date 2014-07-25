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

#include <Burngine/Graphics/General/OpenGL.h>

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
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);    //No resizable window
	window = glfwCreateWindow(	static_cast<int>(settings.getWidth()),
								static_cast<int>(settings.getHeight()),
								settings.getTitle().c_str(),
								settings.isFullscreen() ? 	glfwGetPrimaryMonitor() :
															0,    //Pass the primary monitor if we want fullscreen
								_fakeWindow != nullptr ? 	_fakeWindow :
															0);

	//Check if creation succeeded
	if(window == nullptr){
		Log::log("Failed to create window!", Log::ERROR);
		glfwTerminate();
		exit(4);
	}else{
		//Store window in list
		_windows.push_back(window);
		_preferredWindow = window;
		_contextEnsured = false;    //Use the new window, not the old context
	}

	ensureContext();

	return window;
}

WindowSettings ContextHandler::estimateWindowResolution(const WindowSettings& ws) {

	//We need GLFW here
	ensureGlfw();

	//No need to estimate a resolution. Use the set resolution
	if(!ws.isFullscreen())
		return ws;

	//We don't want the highest res., but ours -> return
	if(!ws.isUsingHighestResolution())
		return ws;

	//We want fullscreen with the best resolution possible
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if(vidmode == nullptr){
		//Failed to get information about the video mode
		return ws;
	}

	WindowSettings newWs(ws);

	//Overwrite the res. of settings with the highest ones
	newWs.setWidth(vidmode->width);
	newWs.setHeight(vidmode->height);

	return newWs;

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
		_contextEnsured = false;    //Find a new one, as the old one might be the destroyed window
	}

	//Finally destroy the window
	glfwDestroyWindow(window);

	//Done :)
}

void ContextHandler::ensureGlfw() {

	//Is GLFW already initialized?
	if(_isGlfwInitialized)
		return;

	//Initialize GLFW
	if(!glfwInit()){
		Log::log("Failed to initialize GLFW!", Log::ERROR);
		exit(3);
	}

	_isGlfwInitialized = true;
}

}
