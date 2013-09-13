/*
 * Material.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include "Material.h"

namespace burn {

Material::Material() :
				_type(SOLID_COLOR) {
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

} /* namespace burn */
