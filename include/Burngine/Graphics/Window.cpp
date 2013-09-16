/*
 * Window.cpp
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

#include "Window.h"
#include <iostream>
#include <unistd.h>

namespace burn {

bool Window::_isContextCreated = false;

Window::Window() :
				_isGlfwInit(false),
				_window(nullptr),
				_framerateLimit(0),
				_elapsedTime(0),
				_lastTime(0),
				_vertexArrayID(0) {
}

Window::~Window() {
	close();
	glfwTerminate();
}

bool Window::create(const WindowSettings& settings) {

	close();
	_settings = settings;

	if(!_isGlfwInit && !glfwInit()){
		std::cout << "Failed to init GLFW!\n";
		return false;
	}else{
		std::cout << "GLFW successfully initialized.\n";
		_isGlfwInit = true;
	}

	std::cout << "-------------------------\n" << glfwGetVersionString() << "\n-------------------------\n";

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //No resizable window

	//The following will get checked after creation
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //We want OpenGL 3.3 at minimum
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	std::cout << "All window-hints set. Attempting creation...\n";

	_window = glfwCreateWindow(static_cast<int>(_settings.getWidth()), static_cast<int>(_settings.getHeight()),
			_settings.getTitle().c_str(), 0, 0);

	if(_window == nullptr){
		glfwTerminate();
		std::cout << "Failed to create window!\n";
		return false;
	}else{
		std::cout << "Window successfully created.\n";
	}

	glfwMakeContextCurrent(_window);
	_isContextCreated = true;

	std::cout << "Setting GLEW to experimental...\n";
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK){
		std::cout << "Failed to init GLEW!\n";
		return false;
	}else{
		std::cout << "GLEW successfully initialited.\n";
	}

	//Check OpenGL version
	if(GLEW_VERSION_4_3){
		std::cout << "OpenGL 4.3 supported\n";
	}else if(GLEW_VERSION_4_2){
		std::cout << "OpenGL 4.2 supported\n";
	}else if(GLEW_VERSION_4_1){
		std::cout << "OpenGL 4.1 supported\n";
	}else if(GLEW_VERSION_4_0){
		std::cout << "OpenGL 4.0 supported\n";
	}else if(GLEW_VERSION_3_3){
		std::cout << "OpenGL 3.3 supported\n";
	}else{
		std::cout << "OpenGL 3.3 is not supported! Check you videocard's driver!\n";
		return false;
	}

	glGenVertexArrays(1, &_vertexArrayID);
	glBindVertexArray(_vertexArrayID);

	std::cout << "Created default VAO...\nWindow creation done.\n";

	return true;
}

bool Window::close() {
	if(_window != nullptr){
		_isContextCreated = false;
		glfwDestroyWindow(_window);
		_window = nullptr;
		return true;
	}
	return false;
}

bool Window::keepOpened() const {
	return !glfwWindowShouldClose(_window);
}

void Window::update() {
	glfwPollEvents();
}

void Window::clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display() {
	double now = glfwGetTime();
	_elapsedTime = now - _lastTime;
	_lastTime = now;

	if(_framerateLimit != 0){
		if(_elapsedTime < (1.0 / _framerateLimit)){
			usleep(((1.0 / _framerateLimit) - _elapsedTime) * 1000000);
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

const double& Window::getElapsedTime() const {
	return _elapsedTime;
}

} /* namespace burn */
