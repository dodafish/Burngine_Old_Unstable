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
#include "Scene.h"

#include <vector>

template class BURNGINE_API std::vector<std::shared_ptr<burn::Scene>>;

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
	 *
	 * @return Returns true on success
	 *
	 * @note This function will destroy the window created by this class
	 * if needed. If you like to have multiple windows, create multiple classes
	 * of type Window and call create() on each.
	 *
	 * @see close()
	 */
	bool create(const WindowSettings& ws = WindowSettings());

	/**
	 * @brief Destroys the current window
	 *
	 * @return Returns true on success
	 *
	 * @see create()
	 */
	bool close();

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
	void update();

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

	/**
	 * @brief Returns the time in seconds which was needed to draw the
	 * last frame.
	 * This is useful to make calculations framerate-independing but depending
	 * on time.
	 *
	 * @return The elapsed time of the last frame in seconds
	 */
	const double& getElapsedTime() const;

	void bind() const;

	/**
	 * @brief This function is used to ensure, that an
	 * OpenGL-Context exists. Calling OpenGL-Methods will result
	 * in crashes when no context is created.
	 * This is internally used by all classes that call OpenGL-Methods
	 *
	 * A Context is created as soon as a window was created.
	 */
	static inline bool isContextCreated();

	enum BlendMode {
		OVERWRITE, ADD, MULTIPLY
	};

	/**
	 * @brief This is used to set the blending mode when rendering.
	 * Initially OVERWRITE is used. So everything you draw will ignore,
	 * what was already drawn.
	 * @note Setting the blendmode will only work, when an OpenGL context
	 * is created!
	 * @see isContextCreated()
	 */
	static inline void setBlendMode(const BlendMode& blendMode);

	std::shared_ptr<Scene> createScene();
	void removeScene(const std::shared_ptr<Scene>& scene);

private:
	std::vector<std::shared_ptr<Scene>> _scenes;

	static bool _isContextCreated;
	bool _isGlfwInit;
	GLFWwindow* _window;
	WindowSettings _settings;

	unsigned int _framerateLimit;
	double _elapsedTime, _lastTime;

	GLuint _vertexArrayID;
};

bool Window::isContextCreated() {
	return (_isContextCreated);
}

void Window::setBlendMode(const Window::BlendMode& blendMode) {
	if(isContextCreated()){
		if(blendMode == OVERWRITE){
			glBlendFunc(GL_ONE, GL_ZERO);
		}else if(blendMode == ADD){
			glBlendFunc(GL_ONE, GL_ONE);
		}else if(blendMode == MULTIPLY){
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		}
	}
}

} /* namespace burn */
#endif /* WINDOW_H_ */
