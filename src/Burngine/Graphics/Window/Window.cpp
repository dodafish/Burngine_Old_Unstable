/*
 * Window.cpp
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

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

bool Window::_isContextCreated = false;
glm::mat4 Window::_orthoMatrix;

Window::Window() :
_isGlfwInit(false),
_window(nullptr),
_framerateLimit(0),
_vertexArrayID(0) {
}

Window::~Window() {

	close();
	glfwTerminate();
}

bool Window::create(const WindowSettings& settings, bool loadShaders) {

	close();
	_settings = settings;

	if(!_isGlfwInit && !glfwInit()){
		Reporter::report("Failed to init GLFW!", Reporter::ERROR);
		return false;
	}else{
		Reporter::report("GLFW successfully initialized.");
		_isGlfwInit = true;
	}

	std::stringstream ss(glfwGetVersionString());
	Reporter::report("Version string: " + ss.str());

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //No resizable window

	//The following will be checked after creation
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //We want OpenGL 3.3 at minimum
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//glfwWindowHint(GLFW_SAMPLES, 8);

	Reporter::report("All window-hints set. Attempting creation...");

	bool validVidmodeInformation = true;
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if(vidmode == nullptr){
		validVidmodeInformation = false;
	}

	if(!_settings.isFullscreen()){
		_window = glfwCreateWindow(static_cast<int>(_settings.getWidth()), static_cast<int>(_settings.getHeight()),
									_settings.getTitle().c_str(), 0, 0);
	}else{
		if(!validVidmodeInformation || !_settings.isUsingHighestResolution()){
			_window = glfwCreateWindow(static_cast<int>(_settings.getWidth()), static_cast<int>(_settings.getHeight()),
										_settings.getTitle().c_str(), glfwGetPrimaryMonitor(), 0);
		}else{
			_window = glfwCreateWindow(vidmode->width, vidmode->height, _settings.getTitle().c_str(),
										glfwGetPrimaryMonitor(), 0);
			_settings.setWidth(vidmode->width);
			_settings.setHeight(vidmode->height);
		}
	}

	if(_window == nullptr){
		glfwTerminate();
		Reporter::report("Failed to create window!", Reporter::ERROR);
		return false;
	}else{
		Reporter::report("Window successfully created.");
	}

	glfwMakeContextCurrent(_window);
	_isContextCreated = true;

	Reporter::report("Setting GLEW to experimental...");
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK){
		Reporter::report("Failed to init GLEW!", Reporter::ERROR);
		return false;
	}else{
		Reporter::report("GLEW successfully initialized.");
	}

	//Checks, if OpenGL 3.3+ is supported
	if(!checkOpenGLVersion()){
		return false;
	}

	//Inputhandling connection to callbacks
	glfwSetKeyCallback(_window, Keyboard::keyCallback);
	glfwSetMouseButtonCallback(_window, Mouse::buttonCallback);
	glfwSetCursorPosCallback(_window, Mouse::cursorPosCallback);

	if(loadShaders){
		if(!BurngineShaders::loadAllShaders()){
			return false;
		}
		Reporter::report("Loaded BurngineShaders.");
	}else{
		Reporter::report("BurngineShaders not loaded! This might cause crashes, when not loaded manually.",
							Reporter::WARNING);
	}

	glGenVertexArrays(1, &_vertexArrayID);
	glBindVertexArray(_vertexArrayID);

	Reporter::report("Created default VAO. Window creation done.");

	//Set default (for Burngine) OpenGL settings
	OpenGlControl::useSettings(OpenGlControl::Settings());

	updateOrthoMatrix();

	_uptime.reset();

	return true;
}

bool Window::checkOpenGLVersion() {

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
		Reporter::report("OpenGL 3.3 is not supported! Try updating your videocard's driver.", Reporter::ERROR);
		return false;
	}

	return true;

}

const WindowSettings& Window::getSettings() const {
	return _settings;
}

bool Window::close() {
	if(_window != nullptr){
		_isContextCreated = false;
		glfwDestroyWindow(_window);
		_window = nullptr;

		std::stringstream ss;
		ss << _uptime.getElapsedTime().asSeconds() << " seconds (" << _uptime.getElapsedTime().asMilliseconds()
		<< " milliseconds; " << _uptime.getElapsedTime().asNanoseconds() << " nanoseconds)";
		Reporter::report("Window closed. Uptime: " + ss.str());
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
	_elapsedTime = _clock.reset();

	if(_framerateLimit != 0){
		if(_elapsedTime.asMicroseconds() < (1000000 / _framerateLimit)){
			usleep(((1000000 / _framerateLimit) - _elapsedTime.asMicroseconds()));
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

const Time& Window::getElapsedTime() const {
	return _elapsedTime;
}

glm::mat4 Window::getOrthoMatrix() {
	return _orthoMatrix;
}

void Window::updateOrthoMatrix() {
	_orthoMatrix = glm::ortho(0.f, static_cast<float>(_settings.getWidth()), 0.f,
								static_cast<float>(_settings.getHeight()));
}

void Window::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _settings.getWidth(), _settings.getHeight());
}

void Window::setCursorPosition(const Vector2d& position) const {
	if(_window != nullptr){
		glfwSetCursorPos(_window, position.x, position.y);
	}
}

} /* namespace burn */
