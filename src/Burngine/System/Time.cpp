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

#include <Burngine/System/Time.h>

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
