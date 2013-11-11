//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#include <Burngine/Graphics/Window/WindowSettings.h>

namespace burn {

WindowSettings::WindowSettings(const unsigned int& width, const unsigned int& height, const std::string& title,
const bool& fullscreen) :
_width(width),
_height(height),
_title(title),
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
