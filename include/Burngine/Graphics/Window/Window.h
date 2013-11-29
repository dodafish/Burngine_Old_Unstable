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

#ifndef WINDOW_H_
#define WINDOW_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/Graphics/Window/WindowSettings.h>

#include <vector>
#include <memory>

#include <Burngine/System/Math.h>
#include <Burngine/System/Clock.h>

namespace burn {

/**
 * @brief The most important class of Burngine.
 * It defines the OpenGL-Context which is needed
 * for all graphical commands.
 */
class BURNGINE_API Window {
public:
	/**
	 * @brief The default constructor
	 */
	Window();

	/**
	 * @brief The default destructor
	 */
	~Window();

	/**
	 * @brief Creates a window defined by WindowSettings.
	 *
	 * @param ws The WindowSettings which the window should be
	 * created to accordingly.
	 * @param loadShaders Loads the predefined BurngineShaders.
	 * If you set this to false, pay attention loading them manually
	 * before drawing anything, that uses no costum shaders!
	 *
	 * @return Returns true on success
	 *
	 * @note This function will destroy the window created by this class
	 * if needed. If you like to have multiple windows, create multiple classes
	 * of type Window and call create() on each.
	 *
	 * @see close()
	 */
	bool create(const WindowSettings& ws = WindowSettings(),
				bool loadShaders = true);

	const WindowSettings& getSettings() const;

	/**
	 * @brief Destroys the current window
	 *
	 * @see create()
	 */
	void close();

	/**
	 * @brief This call will check events which want to close the window
	 *
	 * @return Returns false if the window should be closed.
	 *
	 * @see update()
	 *
	 * @note Events will be updated by calling update()
	 */
	bool keepOpened() const;

	/**
	 * @brief Updates and handles all events. This must be called
	 * again and again in order to tell the OS that the program
	 * is not stuck and running fine.
	 *
	 * @see keepOpened()
	 */
	static void update();

	/**
	 * @brief Cleares the back buffer. (The buffer on which Burngine is
	 * rendering)
	 */
	void clear() const;

	/**
	 * @brief Swaps the back buffer with the front buffer. Due to the call
	 * the buffer on which had been drawn will be displayed.
	 *
	 * @note This method takes the framerate limit in account.
	 */
	void display();

	/**
	 * @brief Caps the framerate of the window.
	 * Set 0 (default) to disable the framerate limit.
	 *
	 * @param fps Frames per second
	 *
	 * @see display()
	 * @see getFramerateLimit()
	 */
	void setFramerateLimit(const unsigned int& fps);

	/**
	 * @brief Returns the current framerate limit of the window
	 *
	 * @return The framerate limit or 0 when no limit is set
	 *
	 * @see setFramerateLimit()
	 */
	const unsigned int& getFramerateLimit() const;

	void setCursorPosition(const Vector2d& position) const;

	void bind() const;

	static glm::mat4 getOrthoMatrix();

	enum PolygonMode {
		FILLED, LINE, POINT
	};

	void setPolygonMode(const PolygonMode& mode) const;

private:
	void updateOrthoMatrix();
	bool checkOpenGLVersion();
	void estimateWindowResolution();

	GLFWwindow* _window;
	WindowSettings _settings;

	unsigned int _framerateLimit;
	Clock _clock; ///< Measures the time between frames
	Time _elapsedTime;

	Clock _uptime; ///< Measures the time the window exists

	GLuint _vertexArrayID;

	static glm::mat4 _orthoMatrix;
};

} /* namespace burn */
#endif /* WINDOW_H_ */
