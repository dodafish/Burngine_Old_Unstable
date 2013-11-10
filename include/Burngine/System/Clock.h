//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

#ifndef CLOCK_H_
#define CLOCK_H_

#include <Burngine/Export.h>
#include <Burngine/System/Time.h>

namespace burn {

/**
 * @brief High-precision clock
 */
class BURNGINE_API Clock {
public:
	/**
	 * @brief Resets the measured time. Also returns the time
	 * elapsed until the call to this method
	 *
	 * @return The time elapsed
	 */
	Time reset();

	/**
	 * @brief Returns the time that elapsed since the last call
	 * to reset()
	 *
	 * @return The time elapsed
	 */
	Time getElapsedTime() const;

private:
	Time _time;
};

} /* namespace burn */
#endif /* CLOCK_H_ */
