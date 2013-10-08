/*
 * Texture.cpp
 *
 *  Created on: 14.09.2013
 *      Author: Dominik
 */

#include "Texture.h"

#include "Window.h"

#include <iostream>

namespace burn {

Texture::Texture() {
}

Texture::~Texture() {
}

bool Texture::loadFromFile(const std::string& file) {

	if(!Window::isContextCreated())
		return false;

	generate();

	_texture = SOIL_load_OGL_texture(
	file.c_str(), SOIL_LOAD_AUTO, _texture,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	if(isCreated()){
		std::cout << "Created texturebuffer. ID: " << _texture << "\n";
		return true;
	}

	return false;
}

} /* namespace burn */
