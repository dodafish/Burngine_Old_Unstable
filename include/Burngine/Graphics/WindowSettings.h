/*
 * WindowSettings.h
 *
 *  Created on: 05.09.2013
 *      Author: Dominik
 */

#ifndef WINDOWSETTINGS_H_
#define WINDOWSETTINGS_H_

#include "../Export.h"
#include <string>

namespace burn {

/**
 * @bried Describes the Window
 */
class BURNGINE_API WindowSettings {
public:
	/**
	 * @brief The default constructor.
	 * You can pass values now or let them get set to
	 * default.
	 */
	WindowSettings(const unsigned int& width = 400, const unsigned int& height = 300, const std::string& title = "",
	const bool& fullscreen = false);

	/**
	 * @brief The default destructor
	 */
	~WindowSettings();

	/**
	 * @brief Sets the width of the window.
	 *
	 * @param width The width. A value greater than 0.
	 *
	 * @see getWidth()
	 *
	 * @note The width actual represents the space in which Burngine draws
	 * the Scene. In other words: The width without any window-decorations.
	 */
	void setWidth(const unsigned int& width);

	/**
	 * @brief Sets the height of the window.
	 *
	 * @param height The height. A value greater than 0.
	 *
	 * @see getHeight()
	 *
	 * @note The height actual represents the space in which Burngine draws
	 * the Scene. In other words: The height without any window-decorations.
	 */
	void setHeight(const unsigned int& height);

	/**
	 * @brief The window-title which is written into the headline of the window
	 *
	 * @param title The window-title
	 *
	 * @see getWindowTitle()
	 */
	void setTitle(const std::string& title);

	/**
	 * @brief Toggles the window between fullscreen- and windowedmode.
	 *
	 * @param fullscreen Set this to true if you want fullscreenmode.
	 *
	 * @see isFullscreen()
	 */
	void setFullscreen(const bool& fullscreen = true);

	/**
	 * @brief Returns the current width of the window
	 *
	 * @return The current width
	 *
	 * @see setWidth()
	 */
	const unsigned int& getWidth() const;

	/**
	 * @brief Returns the current height of the window
	 *
	 * @return The current height
	 *
	 * @see setHeight()
	 */
	const unsigned int& getHeight() const;

	/**
	 * @brief Return the current title of the window
	 *
	 * @return The window-title
	 *
	 * @see setTitle()
	 */
	const std::string& getTitle() const;

	/**
	 * @brief Return the mode of the window
	 *
	 * @return Returns true when window is in fullscreenmode
	 *
	 * @see setFullscreen()
	 */
	const bool& isFullscreen() const;

	void useHighestResolution(const bool& useHighestPossible = true);
	const bool& isUsingHighestResolution() const;

private:
	unsigned int _width, _height;
	std::string _title;
	bool _isFullscreen, _useHighestResolution;
};

} /* namespace burn */
#endif /* WINDOWSETTINGS_H_ */
