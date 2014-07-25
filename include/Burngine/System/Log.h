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

#ifndef REPORTER_H_
#define REPORTER_H_

#include <Burngine/Export.h>

namespace burn {

class BURNGINE_API Log {
public:

	enum LogType {
		NOTIFICATION = 0, WARNING, ERROR
	};

	/**
	 * @brief Reporter is a static class
	 */
	Log() = delete;

	static void log(const std::string& text,
					const LogType& type = NOTIFICATION);

	static void logToConsole(bool enabled = true);
	static void logToFile(bool enabled = true);
	static void enableLogging(bool enabled = true);

private:
	static std::string doubleDigit(const int& number);

	static void toConsole(const std::string& text);
	static void toFile(const std::string& text);
	static std::string timestamp();

	static bool _firstDump;    ///< To be able to mark beginnings
	static bool _logFile, _logConsole;
	static bool _logOff;
};

} /* namespace burn */
#endif /* REPORTER_H_ */
