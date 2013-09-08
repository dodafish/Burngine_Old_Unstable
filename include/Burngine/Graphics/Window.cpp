/*
 * Window.cpp
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

#include "Window.h"
#include <iostream>

namespace burn {

Window::Window() :
				_window(nullptr) {
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

void Window::update(){
	glfwPollEvents();
}

void Window::display() const {
	glfwSwapBuffers(_window);
}

} /* namespace burn */
