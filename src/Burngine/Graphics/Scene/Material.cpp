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

#include <Burngine/Graphics/Scene/Material.h>

namespace burn {

Material::Material() :
_type(SOLID_COLOR),
_specularColor(Vector3f(1.f, 1.f, 1.f)),
_diffuseColor(Vector3f(1.f, 1.f, 1.f)) {

	_flags[VERTEX_ORDER_CLOCKWISE] = false;
	_flags[DRAW_Z_BUFFER] = true;
	_flags[CAST_SHADOWS] = true;

}

Material::Material(const Material& other) :
_type(other._type),
_specularColor(other._specularColor),
_diffuseColor(other._diffuseColor) {

	_flags[VERTEX_ORDER_CLOCKWISE] = other._flags[VERTEX_ORDER_CLOCKWISE];
	_flags[DRAW_Z_BUFFER] = other._flags[DRAW_Z_BUFFER];
	_flags[CAST_SHADOWS] = other._flags[CAST_SHADOWS];

}

Material& Material::operator=(const Material& other) {

	if(this == &other)
		return *this;

	_type = other._type;
	_specularColor = other._specularColor;
	_diffuseColor = other._diffuseColor;

	_flags[VERTEX_ORDER_CLOCKWISE] = other._flags[VERTEX_ORDER_CLOCKWISE];
	_flags[DRAW_Z_BUFFER] = other._flags[DRAW_Z_BUFFER];
	_flags[CAST_SHADOWS] = other._flags[CAST_SHADOWS];

	return *this;
}

void Material::setFlag(	Material::Flag flag,
						bool enabled) {
	if(flag != COUNT){
		_flags[flag] = enabled;
	}
}

void Material::setType(Material::Type type) {
	_type = type;
}

const Material::Type& Material::getType() const {
	return _type;
}

bool Material::isFlagSet(Material::Flag flag) const {
	if(flag != COUNT){
		return _flags[flag];
	}
	return false;
}

void Material::setSpecularColor(const Vector3f& color) {
	_specularColor = color;
}

const Vector3f& Material::getSpecularColor() const {
	return _specularColor;
}

void Material::setDiffuseColor(const Vector3f& color) {
	_diffuseColor = color;
}

const Vector3f& Material::getDiffuseColor() const {
	return _diffuseColor;
}

} /* namespace burn */
