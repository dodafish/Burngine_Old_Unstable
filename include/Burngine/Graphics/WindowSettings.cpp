/*
 * WindowSettings.cpp
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

#include "WindowSettings.h"

namespace burn {

WindowSettings::WindowSettings() :
		_title("") {
	// TODO Auto-generated constructor stub

}

WindowSettings::~WindowSettings() {
	// TODO Auto-generated destructor stub
}

void WindowSettings::setTitle(const std::string& title) {
	_title = title;
}

const std::string& WindowSettings::getTitle() const {
	return _title;
}

} /* namespace burn */
