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

#include <Burngine/Graphics/Window/Window.h>
#include <iostream>
#include <unistd.h>

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <Burngine/System/Keyboard.h>
#include <Burngine/System/Mouse.h>
#include <Burngine/System/Reporter.h>
#include <sstream>

namespace burn {

glm::mat4 Window::_orthoMatrix;

Window::Window() :
_window(nullptr),
_framerateLimit(0),
_vertexArrayID(0) {
}

Window::~Window() {
	close();
}

bool Window::create(const WindowSettings& settings,
					bool loadShaders) {

	close();
	_settings = settings;

	//Log information
	std::stringstream ss(glfwGetVersionString());
	Reporter::report("Version string: " + ss.str());

	// Set the window resolution according to _settings
	estimateWindowResolution();

	//Create Window with ContextHandler :)
	//This returns only on success, program will get terminated otherwise
	_window = ContextHandler::createWindow(_settings);

	//Inputhandling connection to callbacks
	glfwSetKeyCallback(_window, Keyboard::keyCallback);
	glfwSetMouseButtonCallback(_window, Mouse::buttonCallback);
	glfwSetCursorPosCallback(_window, Mouse::cursorPosCallback);

	if(loadShaders){
		if(!BurngineShaders::load("Shaders/")){
			return false;
		}
		Reporter::report("Loaded BurngineShaders.");
	}else{
		Reporter::report(	"BurngineShaders not loaded! This might cause crashes, when not loaded manually.",
							Reporter::WARNING);
	}

	//////////////////////////////////////////////////////////////
	// Window creation succeeded. Setup OpenGL a little bit
	//////////////////////////////////////////////////////////////

	glGenVertexArrays(1, &_vertexArrayID);
	glBindVertexArray(_vertexArrayID);

	Reporter::report("Created default VAO. Window creation done.");

	//Set default (for Burngine) OpenGL settings
	OpenGlControl::useSettings(OpenGlControl::Settings());

	updateOrthoMatrix();

	_uptime.reset();

	return true;
}

void Window::estimateWindowResolution() {

	//No need to estimate a resolution. Use the set resolution
	if(!_settings.isFullscreen())
		return;

	//We don't want the highest res., but ours -> return
	if(!_settings.isUsingHighestResolution())
		return;

	//We want fullscreen with the best resolution possible
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if(vidmode == nullptr){
		//Failed to get information about the video mode
		return;
	}

	//Overwrite the res. of settings with the highest ones
	_settings.setWidth(vidmode->width);
	_settings.setHeight(vidmode->height);

}

const WindowSettings& Window::getSettings() const {
	return _settings;
}

void Window::close() {

	if(_window == nullptr)
		return;

	ContextHandler::destroyWindow(_window);
	_window = nullptr;

	std::stringstream ss;
	ss << _uptime.getElapsedTime().asSeconds() << " seconds (" << _uptime.getElapsedTime().asMilliseconds()
	<< " milliseconds; " << _uptime.getElapsedTime().asNanoseconds() << " nanoseconds)";
	Reporter::report("Window closed. Uptime: " + ss.str());

}

bool Window::keepOpened() const {
	return !glfwWindowShouldClose(_window);
}

void Window::update() {
	glfwPollEvents();
}

void Window::clear() const {

	if(_window == nullptr)
		return;

	ContextHandler::useContext(_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display() {

	if(_window == nullptr)
		return;

	ContextHandler::useContext(_window);


	if(_framerateLimit != 0){
		_elapsedTime = _clock.reset();
		if(_elapsedTime.asMicroseconds() < (1000000.0 / _framerateLimit)){
			usleep(((1000000.0 / _framerateLimit) - _elapsedTime.asMicroseconds()));
		}
	}

	glfwSwapBuffers(_window);
}

void Window::setFramerateLimit(const unsigned int& fps) {
	_framerateLimit = fps;
}

const unsigned int& Window::getFramerateLimit() const {
	return _framerateLimit;
}

glm::mat4 Window::getOrthoMatrix() {
	return _orthoMatrix;
}

void Window::updateOrthoMatrix() {
	_orthoMatrix = glm::ortho(	0.f,
								static_cast<float>(_settings.getWidth()),
								0.f,
								static_cast<float>(_settings.getHeight()));
}

void Window::bind() const {

	if(_window == nullptr)
		return;

	ContextHandler::useContext(_window);

	OpenGlControl::bindDrawBuffer(0, true);
	glViewport(0, 0, static_cast<int>(_settings.getWidth()), static_cast<int>(_settings.getHeight()));

}

void Window::setCursorPosition(const Vector2d& position) const {
	if(_window != nullptr){
		glfwSetCursorPos(_window, position.x, position.y);
	}
}

void Window::setPolygonMode(const PolygonMode& mode) const {

	ContextHandler::useContext(_window);

	if(mode == FILLED)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if(mode == LINE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

} /* namespace burn */
