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

#include <Burngine/Graphics/Scene/Light.h>
#include <Burngine/System/Message.h>

namespace burn {

Light::Light() :
_color(Vector3f(1.f)),
_intensity(10.f),
_isSofteningShadow(false) {

}

Light::Light(const Light& other) :
_color(other._color),
_intensity(other._intensity),
_isSofteningShadow(other._isSofteningShadow) {

}

Light& Light::operator=(const Light& other) {

	if(this == &other)
		return *this;

	_color = other._color;
	_intensity = other._intensity;
	_isSofteningShadow = other._isSofteningShadow;

	return *this;

}

Light::~Light() {
	Message msg(mn::LIGHT_DESTRUCTED);
	msg.addParameter<Uint64>(mp::COMPONENT_ID, _id.get());
	msg.send();
}

void Light::setColor(const Vector3f& color) {
	_color = color;
}

const Vector3f& Light::getColor() const {
	return _color;
}

void Light::setIntensity(const float& intensity) {
	_intensity = (intensity > 0) ? 	intensity :
									0;
}

const float& Light::getIntensity() const {
	return _intensity;
}

void Light::softenShadow(bool enabled) {
	_isSofteningShadow = enabled;
}

bool Light::isSofteningShadow() const {
	return _isSofteningShadow;
}

} /* namespace burn */
