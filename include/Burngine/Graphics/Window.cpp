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
				_lastTime(0) {
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
		_isGlfwInit = true;
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //No resizable window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //We want OpenGL 3.3 at minimum
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	_window = glfwCreateWindow(static_cast<int>(_settings.getWidth()), static_cast<int>(_settings.getHeight()),
			_settings.getTitle().c_str(), 0, 0);

	if(_window == nullptr){
		glfwTerminate();
		std::cout << "Failed to create window!\n";
		return false;
	}

	glfwMakeContextCurrent(_window);
	_isContextCreated = true;

	if(glewInit() != GLEW_OK){
		std::cout << "Failed to init GLEW!\n";
		return false;
	}

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
