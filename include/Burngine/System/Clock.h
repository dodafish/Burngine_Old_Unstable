/*
 * Clock.h
 *
 *  Created on: 05.10.2013
 *      Author: Dominik
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "../Export.h"
#include "Time.h"

namespace burn {

class BURNGINE_API Clock {
public:
	Clock();

	Time reset();

	Time getElapsedTime() const;

private:
	Time _time;
};

} /* namespace burn */
#endif /* CLOCK_H_ */
