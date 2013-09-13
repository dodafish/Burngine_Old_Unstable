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
				_window(nullptr),
				_framerateLimit(0),
				_elapsedTime(0) {
}

Window::~Window() {
}

bool Window::isContextCreated() {
	return (_isContextCreated);
}

bool Window::create(const WindowSettings& settings) {

	close();
	_settings = settings;

	if(!glfwInit()){
		std::cout << "Failed to init GLFW!\n";
		return false;
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
	_elapsedTime = glfwGetTime();

	if(_framerateLimit != 0){
		double _elapsedMicro = _elapsedTime * 1000000;
		double frameTime = 1000000.0 / _framerateLimit;
		if(_elapsedMicro < frameTime){
			usleep(frameTime - _elapsedMicro);
		}
	}

	glfwSetTime(0);
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
