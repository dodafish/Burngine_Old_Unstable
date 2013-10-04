/*
 * WindowSettings.cpp
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

#include "WindowSettings.h"

namespace burn {

WindowSettings::WindowSettings(const unsigned int& width, const unsigned int& height, const std::string& title,
const bool& fullscreen) :
_width(width),
_height(height),
_title("Burngine"),
_isFullscreen(fullscreen),
_useHighestResolution(true) {

}

WindowSettings::~WindowSettings() {
	// TODO Auto-generated destructor stub
}

void WindowSettings::setWidth(const unsigned int& width) {
	_width = width;
}

void WindowSettings::setHeight(const unsigned int& height) {
	_height = height;
}

void WindowSettings::setTitle(const std::string& title) {
	_title = title;
}

void WindowSettings::setFullscreen(const bool& fullscreen) {
	_isFullscreen = fullscreen;
}

const unsigned int& WindowSettings::getWidth() const {
	return _width;
}

const unsigned int& WindowSettings::getHeight() const {
	return _height;
}

const std::string& WindowSettings::getTitle() const {
	return _title;
}

const bool& WindowSettings::isFullscreen() const {
	return _isFullscreen;
}

void WindowSettings::useHighestResolution(const bool& useHighestPossible) {
	_useHighestResolution = useHighestPossible;
}

const bool& WindowSettings::isUsingHighestResolution() const {
	return _useHighestResolution;
}

} /* namespace burn */
