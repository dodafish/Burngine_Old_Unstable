/*
 * Rectangle.h
 *
 *  Created on: 17.10.2013
 *      Author: Dominik
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

namespace burn {

template<typename T>
class BURNGINE_API Rectangle {
public:
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
