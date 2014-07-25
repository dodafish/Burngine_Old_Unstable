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

#include <Burngine/System/Log.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>

namespace burn {

bool Log::_firstDump = true;
bool Log::_logConsole = false;
bool Log::_logFile = false;
bool Log::_logOff = false;

void Log::logToConsole(bool enabled) {
	_logConsole = enabled;
}
void Log::logToFile(bool enabled) {
	_logFile = enabled;
}
void Log::enableLogging(bool enabled) {
	_logOff = !enabled;
}

void Log::toConsole(const std::string& text) {
	std::cout << text;
}

void Log::toFile(const std::string& text) {
	std::ofstream out;

	out.open("BurngineLog.txt", std::ios::app);
	if(out.is_open()){

		std::string ts = timestamp();

		if(_firstDump){
			std::stringstream strs;
			strs << __DATE__ << " " << __TIME__;

			out << "\n\n////////////////////////////////////////\n";
			out << "Beginning of report: " + ts + "\n";
			out << "Compilation of Burngine: " + strs.str() + "\n";
			out << "////////////////////////////////////////\n";
			_firstDump = false;
		}
		out << ts << ": " << text << "\n";

		out.close();
	}

}

std::string Log::timestamp() {
	time_t t = time(NULL);
	struct tm * now = localtime(&t);
	std::ostringstream ss;
	ss << (now->tm_year + 1900);
	return (doubleDigit(now->tm_mday) + "-" + doubleDigit(now->tm_mon) + "-" + ss.str() + " "
	+ doubleDigit(now->tm_hour) + doubleDigit(now->tm_min) + doubleDigit(now->tm_sec));
}

std::string Log::doubleDigit(const int& number) {
	std::ostringstream ss;
	ss << number;
	std::string prefix = "";
	if(number < 10){
		prefix = "0";
	}
	return (prefix + ss.str());
}

void Log::log(	const std::string& text,
				const Log::LogType& type) {

	if(!_logOff){

		if(_logConsole)
			if(type == ERROR)
				toConsole("[ERROR]-> " + text + "\n");
			else if(type == WARNING)
				toConsole("[WARNING]-> " + text + "\n");
			else
				toConsole("[NOTIF.]-> " + text + "\n");

		if(_logFile)
			if(type == ERROR)
				toFile("[ERROR]-> " + text + "\n");
			else if(type == WARNING)
				toFile("[WARNING]-> " + text + "\n");
			else
				toFile("[NOTIF.]-> " + text + "\n");

	}

}

} /* namespace burn */
