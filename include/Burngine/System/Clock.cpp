/*
 * Clock.cpp
 *
 *  Created on: 05.10.2013
 *      Author: Dominik
 */

#include "Clock.h"

namespace burn {

Clock::Clock() {
}

Time Clock::reset() {
	Time elapsed = getElapsedTime();
	_time.setTime(Time::now());
	return elapsed;
}

Time Clock::getElapsedTime() const {
	return (Time() - _time);
}

} /* namespace burn */
