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
				_isTextureGenerated(false),
				_texture(0) {

	generate();

}

Texture::~Texture() {
	erase();
}

bool Texture::loadFromFile(const std::string& file) {

	generate(); //if needed

	_texture = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, _texture,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	return (_texture != 0);
}

const GLuint& Texture::getTextureBuffer() const {
	return _texture;
}

void Texture::generate() {
	if(!_isTextureGenerated && Window::isContextCreated()){
		glGenTextures(1, &_texture);
		_isTextureGenerated = true;
	}
}

void Texture::erase() {
	if(_isTextureGenerated && Window::isContextCreated()){
		glDeleteTextures(1, &_texture);
		_isTextureGenerated = false;
	}
}

} /* namespace burn */
