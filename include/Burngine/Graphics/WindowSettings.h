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

class BURNGINE_API WindowSettings {
public:
	WindowSettings(const unsigned int& width = 400, const unsigned int& height =
			300, const std::string& title = "", const bool& fullscreen = false);

	~WindowSettings();

	void setWidth(const unsigned int& width);
	void setHeight(const unsigned int& height);
	void setTitle(const std::string& title);
	void setFullscreen(const bool& fullscreen = true);

	const unsigned int& getWidth() const;
	const unsigned int& getHeight() const;
	const std::string& getTitle() const;
	const bool& isFullscreen() const;

private:
	unsigned int _width, _height;
	std::string _title;
	bool _isFullscreen;
};

} /* namespace burn */
#endif /* WINDOWSETTINGS_H_ */
