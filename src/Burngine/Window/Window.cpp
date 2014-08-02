//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
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

#include <Burngine/Window/Window.hpp>
#include <Burngine/Window/WindowImpl.hpp>

namespace burn {

Window::Window() :
		m_impl(NULL) {
}

Window::~Window() {

	// Close the window if needed
	close();

}

bool Window::create() {

	// Close a possibly created window before creating
	// a new one
	close();

	// Create a new window
	m_impl = priv::WindowImpl::create();

	return m_impl != NULL;
}

void Window::close() {

	// Delete the window
	delete m_impl;
	m_impl = NULL;

}

bool Window::isOpen() const {
	return m_impl != NULL;
}

bool Window::pollEvent(Event& event) {

	if (m_impl && m_impl->popEvent(event)) {
		return true;
	}

	return false;
}

} /* namespace burn */
