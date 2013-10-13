/*
 * Texture.cpp
 *
 *  Created on: 14.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Texture/Texture.h>

#include <Burngine/Graphics/Window/Window.h>

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

bool Texture::loadFromData(GLubyte* data, const Vector2ui& dimensions, const Int32& bpp, const GLenum& format) {

	//Create Texture on GPU. Dimensions will be power of 2
	create(dimensions);

	//Bind the texture
	bind();

	//Fill with data
	glTexImage2D(GL_TEXTURE_2D, 0, format, getDimensions().x, getDimensions().y, 0, format, GL_UNSIGNED_BYTE, data);

	//Unbind texture to protect against modifications
	unbind();

	return true;
}

} /* namespace burn */
