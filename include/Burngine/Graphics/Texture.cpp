/*
 * Texture.cpp
 *
 *  Created on: 14.09.2013
 *      Author: Dominik
 */

#include "Texture.h"

#include "Window.h"

namespace burn {

Texture::Texture() :
				_texture(0) {

}

Texture::~Texture() {
}

bool Texture::loadFromFile(const std::string& file) {
	if(_texture == 0){
		_texture = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	}else{
		_texture = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, _texture,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	}
	return (_texture != 0);
}

const GLuint& Texture::getTextureBuffer() const {
	return _texture;
}

} /* namespace burn */
