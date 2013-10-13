/*
 * Reporter.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Dominik
 */

#include <Burngine/System/Reporter.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>

namespace burn {

bool Reporter::dumpToFile = true;
bool Reporter::dumpToConsole = true;
std::string Reporter::file = "log.txt";
bool Reporter::firstDump = true;

void Reporter::report(const std::string& msg, const Reporter::MessageType& type) {

	//Generate Timestamp
	time_t t = time(NULL);
	struct tm * now = localtime(&t);
	std::ostringstream ss;
	ss << (now->tm_year + 1900);
	std::string timestamp = doubleDigit(now->tm_mday) + "-" + doubleDigit(now->tm_mon) + "-" + ss.str() + " "
	+ doubleDigit(now->tm_hour) + doubleDigit(now->tm_min) + doubleDigit(now->tm_sec);

	//Open dumping file or set to false when error occurs
	std::ofstream out;
	if(dumpToFile){
		out.open(file.c_str(), std::ios::app);
		if(!out.is_open()){
			dumpToFile = false;
			if(dumpToConsole){
				std::cout << "[ERROR] Cannot dump to file (file cannot be opened)\n";
			}
		}
	}

	//Mark beginning
	if(firstDump && dumpToFile){
		std::stringstream strs;
		strs << __DATE__ << " " << __TIME__;

		out << "\n\n////////////////////////////////////////\n";
		out << "Beginning of report: " + timestamp + "\n";
		out << "Compilation of Burngine: " + strs.str() + "\n";
		out << "////////////////////////////////////////\n";
		firstDump = false;
	}

	switch (type) {
		case NOTIFICATION:
			if(dumpToFile){
				out << timestamp << ": " << msg << "\n";
			}
			if(dumpToConsole){
				std::cout << msg << "\n";
			}
			break;
		case ERROR:
			if(dumpToFile){
				out << timestamp << ": [ERROR] " << msg << "\n";
			}
			if(dumpToConsole){
				std::cout << "[ERROR] " << msg << "\n";
			}
			break;
		case WARNING:
			if(dumpToFile){
				out << timestamp << ": [WARNING] " << msg << "\n";
			}
			if(dumpToConsole){
				std::cout << "[WARNING] " << msg << "\n";
			}
			break;
		default:
			break;
	}

	if(out.is_open())
		out.close();

}

std::string Reporter::doubleDigit(const int& number) {
	std::ostringstream ss;
	ss << number;
	std::string prefix = "";
	if(number < 10){
		prefix = "0";
	}
	return (prefix + ss.str());
}

} /* namespace burn */
