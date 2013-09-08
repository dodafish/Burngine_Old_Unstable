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
	void display() const;

private:
	GLFWwindow* _window;
	WindowSettings _settings;
};

} /* namespace burn */
#endif /* WINDOW_H_ */
