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

#include <Burngine/Graphics/Texture/Texture.h>

#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>

namespace burn {

Texture::Texture() :
_textureId(0),
_referenceCount(new unsigned int(1)) {

}

Texture::Texture(const Texture& other) :
_textureId(other._textureId),
_referenceCount(other._referenceCount) {
	++(*_referenceCount);
}

Texture& Texture::operator=(const Texture& other) {

	if(this == &other)
		return *this;

	if(*_referenceCount < 2){
		cleanup();
		delete _referenceCount;
	}else{
		--(*_referenceCount);
	}

	_textureId = other._textureId;
	_referenceCount = other._referenceCount;

	++(*_referenceCount);

	return *this;
}

Texture::~Texture() {

	if(*_referenceCount < 2){
		cleanup();
		delete _referenceCount;
	}else{
		--(*_referenceCount);
	}

}

void Texture::generate() {
	if(*_referenceCount < 2){
		cleanup();
	}else{
		--(*_referenceCount);
		_referenceCount = new unsigned int(1);
		_textureId = 0;
	}
	glGenTextures(1, &_textureId);

	if(_textureId == 0){
		Reporter::report("Texture creation failed!", Reporter::ERROR);
	}

}

void Texture::cleanup() {

	ensureContext();

	glDeleteTextures(1, &_textureId);

	_textureId = 0;

}

bool Texture::loadFromFile(const std::string& file) {

	ensureContext();

	generate();

	_textureId = SOIL_load_OGL_texture(
	file.c_str(), SOIL_LOAD_AUTO, _textureId,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	return true;
}

bool Texture::loadFromData(GLubyte* data, const Vector2ui& dimensions, const Int32& bpp, const GLenum& format) {

	generate();

	//Bind the texture
	bindAsSource();

	//Fill with data
	if(format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dimensions.x, dimensions.y, 0, format, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, dimensions.x, dimensions.y, 0, format, GL_UNSIGNED_BYTE, data);

	return true;
}

void Texture::onBind(const unsigned int& unit) const {

	ensureContext();

	if(_textureId == 0){
		Reporter::report("Binding uncreated texture. ID: 0!", Reporter::WARNING);
	}

	bindTexture(_textureId, unit);
}

} /* namespace burn */
