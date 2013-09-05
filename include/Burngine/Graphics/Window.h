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

namespace burn {

class Window {
public:
	Window(const WindowSettings& = WindowSettings());

	~Window();

private:

};

} /* namespace burn */
#endif /* WINDOW_H_ */
