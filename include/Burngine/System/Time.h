/*
 * Time.h
 *
 *  Created on: 05.10.2013
 *      Author: Dominik
 */

#ifndef TIME_H_
#define TIME_H_

#include <Burngine/Export.h>
#include <chrono>

namespace burn {

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

class BURNGINE_API Time {
public:
	//Calculations of a time unit to nanoseconds
	static TimePoint seconds(const double& seconds);
	static TimePoint milliseconds(const int& milliseconds);

	Time();
	Time(const TimePoint& point);

	void setTime(const TimePoint& point);

	long double asSeconds() const;
	unsigned long long asMilliseconds() const;
	unsigned long long asMicroseconds() const;
	unsigned long long asNanoseconds() const;

	static TimePoint now();

	Time operator-(const Time& other) const;

	const TimePoint& getTimePoint() const;

private:
	TimePoint _timePoint;
};

} /* namespace burn */
#endif /* TIME_H_ */
