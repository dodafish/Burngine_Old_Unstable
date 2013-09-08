/*
 * Window.cpp
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

#include "Window.h"
#include <iostream>

#include <time.h>

namespace burn {

Window::Window() :
				_window(nullptr),
				_framerateLimit(0),
				_lastTime(0){
}

Window::~Window() {
}

bool Window::create(const WindowSettings& settings) {

	close();
	_settings = settings;

	if(!glfwInit()){
		std::cout << "Failed to init GLFW!\n";
		return false;
	}

	_window = glfwCreateWindow(static_cast<int>(_settings.getWidth()),
			static_cast<int>(_settings.getHeight()),
			_settings.getTitle().c_str(), 0, 0);

	if(_window == nullptr){
		glfwTerminate();
		std::cout << "Failed to create window!\n";
		return false;
	}

	glfwMakeContextCurrent(_window);

	if(glewInit() != GLEW_OK){
		std::cout << "Failed to init GLEW!\n";
		return false;
	}

	return true;
}

bool Window::close() {
	return true;
}

bool Window::keepOpened() const {
	return !glfwWindowShouldClose(_window);
}

void Window::update() {
	glfwPollEvents();
}

void Window::display() const {
	if(_framerateLimit != 0){
		double now = glfwGetTime();
		double delta = now - _lastTime;
		if(delta > 0){
			//TODO wait ms until bufferswap, or better: start a thread waiting for bufferswap and start calculating next scene
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

} /* namespace burn */
