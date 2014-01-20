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
#include <Burngine/System/Reporter.h>

namespace burn {

GLuint Texture::_currentTextureBinding[];
Int32 Texture::_realTextureBindingCap = 0;

Texture::Texture() :
_textureId(0) {

	if(_realTextureBindingCap == 0){
		//Get OpenGL informations about textures
		ensureContext();
		for(int i = 0; i != MAX_TEXTURE_BINDINGS; ++i)
			_currentTextureBinding[i] = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_realTextureBindingCap);

		if(_realTextureBindingCap == 0){
			Reporter::report("Could not estimate maximum number of texture units!", Reporter::ERROR);
			exit(1);    //Kill process
		}

	}
}

Texture::Texture(const Texture& other) :
_textureId(0),
_dimensions(other._dimensions),
_internalFormat(other._internalFormat) {

	ensureContext();

	//Create texture
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D( 	GL_TEXTURE_2D,
					0,
					_internalFormat,
					_dimensions.x,
					_dimensions.y,
					0,
					estimateFormat(_internalFormat),
					estimateType(_internalFormat),
					0);

	//Copy the texture's pixels

}

Texture& Texture::operator=(const Texture& other) {

	if(this == &other)
		return *this;

	return *this;
}

bool Texture::create(	const Vector2ui& dimensions,
						const InternalFormat& internalFormat) {

	_dimensions = dimensions;
	_internalFormat = internalFormat;

	ensureContext();

	//Generate texture if needed
	if(_textureId == 0)
		glGenTextures(1, &_textureId);

	//Assign memory on gpu
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D( 	GL_TEXTURE_2D,
					0,
					_internalFormat,
					_dimensions.x,
					_dimensions.y,
					0,
					estimateFormat(_internalFormat),
					estimateType(_internalFormat),
					0);

	//Restore previous bound texture
	glBindTexture(GL_TEXTURE_2D, _currentTextureBinding[0]);

	return (_textureId != 0);
}

bool Texture::bind(const Uint32& unit) const {

	if(_textureId == 0){
		Reporter::report("Cannot bind texture. Texture has not been created.", Reporter::ERROR);
		return false;
	}
	if(unit > _realTextureBindingCap - 1 || unit > MAX_TEXTURE_BINDINGS - 1){
		Reporter::report("Cannot bind texture. Chosen texture unit too large.", Reporter::ERROR);
		return false;
	}

	ensureContext();

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	_currentTextureBinding[unit] = _textureId;

	return true;
}

void Texture::copyTextureData(const GLuint& src) {

}

} /* namespace burn */
