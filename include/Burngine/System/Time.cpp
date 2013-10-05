/*
 * Time.cpp
 *
 *  Created on: 05.10.2013
 *      Author: Dominik
 */

#include "Time.h"

namespace burn {

Time::Time() :
_timePoint(std::chrono::high_resolution_clock::now()) {

}

Time::Time(const TimePoint& point) :
_timePoint(point) {

}

void Time::setTime(const TimePoint& point) {
	_timePoint = point;
}

long double Time::asSeconds() const {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _timePoint).count()
	/ 1000000000.0;
}

unsigned long long Time::asMilliseconds() const {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _timePoint).count();
}
unsigned long long Time::asMicroseconds() const {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _timePoint).count();
}
unsigned long long Time::asNanoseconds() const {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _timePoint).count();
}

TimePoint Time::now() {
	return std::chrono::high_resolution_clock::now();
}

const TimePoint& Time::getTimePoint() const {
	return _timePoint;
}

Time Time::operator-(const Time& other) const {
	return Time(_timePoint - (_timePoint - other.getTimePoint()));
}

} /* namespace burn */
