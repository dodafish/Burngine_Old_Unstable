/*
 * Reporter.h
 *
 *  Created on: 02.10.2013
 *      Author: Dominik
 */

#ifndef REPORTER_H_
#define REPORTER_H_

#include <Burngine/Export.h>

namespace burn {

class BURNGINE_API Reporter {
public:

	enum MessageType {
		NOTIFICATION = 0, ERROR, WARNING
	};

	/**
	 * @brief Reporter is a static class
	 */
	Reporter() = delete;

	static void report(const std::string& message, const MessageType& type = NOTIFICATION);

private:
	static std::string doubleDigit(const int& number);

	static bool dumpToFile, dumpToConsole; ///< Where to report to
	static std::string file; ///< The "logfile"
	static bool firstDump; ///< To be able to mark beginnings
};

} /* namespace burn */
#endif /* REPORTER_H_ */
