/*
 * Material.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include "Material.h"

namespace burn {

Material::Material() :
_type(SOLID_COLOR),
_specularColor(Vector3f(1.f, 1.f, 1.f)),
_diffuseColor(Vector3f(1.f, 0.7f, 0.f)),
_index(0),
_useDiffuseColor(false) {
	_flags[LIGHTING] = false;
}

Material::~Material() {
}

void Material::setFlag(Material::Flag flag, bool enabled) {
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
	_useDiffuseColor = true;
}

const Vector3f& Material::getDiffuseColor() const {
	return _diffuseColor;
}

void Material::setIndex(const unsigned int& index) {
	_index = index;
}

const unsigned int& Material::getIndex() const {
	return _index;
}

void Material::useDiffuseColor(bool shouldUseDiffuse) {
	_useDiffuseColor = shouldUseDiffuse;
}

bool Material::isUsingDiffuseColor() const {
	return _useDiffuseColor;
}

} /* namespace burn */
