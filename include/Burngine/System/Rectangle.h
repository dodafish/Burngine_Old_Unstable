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

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

namespace burn {

/**
 * @brief Represents a 2D Rectangle (e.g. bounding box for GuiNodes)
 */
template<typename T>
class BURNGINE_API Rectangle {
public:
	/**
	 * @brief Default constructor initializing some values
	 */
	Rectangle();

	T left, bottom, width, height;
};

template<typename T>
Rectangle<T>::Rectangle() :
left(0),
bottom(0),
width(0),
height(0) {
}


//Predefine some Rectangle types
typedef Rectangle<float> FloatRect;
typedef Rectangle<Int32> IntRect;

} /* namespace burn */
#endif /* RECTANGLE_H_ */
