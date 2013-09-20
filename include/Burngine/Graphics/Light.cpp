/*
 * Light.cpp
 *
 *  Created on: 20.09.2013
 *      Author: Dominik
 */

#include "Light.h"

namespace burn {

Light::Light(const Type& type, const Vector3f& color, const float& intensity) :
				_color(color),
				_intensity((intensity > 0) ? intensity : 0),
				_type(type) {

}

Light::~Light() {
}

void Light::setColor(const Vector3f& color) {
	_color = color;
}

const Vector3f& Light::getColor() const {
	return _color;
}

void Light::setIntensity(const float& intensity) {
	_intensity = (intensity > 0) ? intensity : 0;
}

const float& Light::getIntensity() const {
	return _intensity;
}

void Light::setType(const Light::Type& type) {
	_type = type;
}

const Light::Type& Light::getType() const {
	return _type;
}

} /* namespace burn */
