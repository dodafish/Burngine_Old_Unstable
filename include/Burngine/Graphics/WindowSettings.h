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
	WindowSettings();
	~WindowSettings();

	void setTitle(const std::string& title);
	const std::string& getTitle() const ;

private:
	std::string _title;
};

} /* namespace burn */
#endif /* WINDOWSETTINGS_H_ */
