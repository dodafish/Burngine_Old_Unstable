/*
 * Window.h
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "../Export.h"
#include "WindowSettings.h"
#include "OpenGL.h"

namespace burn {

class BURNGINE_API Window {
public:
	Window();

	~Window();

	bool create(const WindowSettings& = WindowSettings());
	bool close();

	bool keepOpened() const;

	void update();
	void clear() const;
	void display();

	void setFramerateLimit(const unsigned int& fps);
	const unsigned int& getFramerateLimit() const;
	const double& getElapsedTime() const;

	static bool isContextCreated();

private:
	static bool _isContextCreated;
	GLFWwindow* _window;
	WindowSettings _settings;

	unsigned int _framerateLimit;
	double _elapsedTime, _lastTime;
};

} /* namespace burn */
#endif /* WINDOW_H_ */
