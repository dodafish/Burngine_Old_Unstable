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

#include <Burngine/extern/SOIL.h>

namespace burn {

void Texture::ensureConstants() {
	if(_realTextureBindingCap == 0){
		//Get OpenGL informations about textures
		ensureContext();
		for(int i = 0; i != MAX_TEXTURE_BINDINGS; ++i)
			_currentTexture2DBinding[i] = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_realTextureBindingCap);

		if(_realTextureBindingCap == 0){
			Reporter::report("Could not estimate maximum number of texture units!", Reporter::ERROR);
			exit(1);    //Kill process
		}

	}
}

GLuint Texture::_currentTexture2DBinding[];

Texture::Texture() :
_textureId(0) {
	ensureConstants();
}

Texture::Texture(	const Vector2ui& dimensions,
					const InternalFormat& internalFormat) :
_textureId(0) {

	ensureConstants();

	//Create texture immediatly
	create(_dimensions, _internalFormat);

}

Texture::~Texture() {
	if(isLastReference())
		cleanup();
}

void Texture::cleanup() {

	if(_textureId != 0){

		//Unbind where necessary
		for(size_t i = 0; i != MAX_TEXTURE_BINDINGS; ++i){
			if(_currentTexture2DBinding[i] == _textureId){
				_currentTexture2DBinding[i] = 0;
			}
		}

		ensureContext();
		glDeleteTextures(1, &_textureId);

		_textureId = 0;
	}

}

bool Texture::create(	const Vector2ui& dimensions,
						const InternalFormat& internalFormat,
						GLubyte* data) {

	if(dimensions.x == 0 || dimensions.y == 0){
		Reporter::report("Unable to create texture. Dimensions must be greater than 0!", Reporter::ERROR);
		return false;
	}

	_dimensions = dimensions;
	_internalFormat = internalFormat;

	ensureContext();

	//Remove old texture and delete opengl entry
	cleanup();
	//Generate texture
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
					data);

	//Restore previous bound texture
	glBindTexture(GL_TEXTURE_2D, _currentTexture2DBinding[0]);

	if(_textureId == 0){
		Reporter::report("Failed to create texture.", Reporter::ERROR);
	}

	return (_textureId != 0);
}

bool Texture::bind(const Uint32& unit) const {

	if(_textureId == 0){
		Reporter::report("Cannot bind texture. Texture has not been created.", Reporter::ERROR);
		return false;
	}
	if(unit > static_cast<Uint32>(_realTextureBindingCap - 1) || unit > static_cast<Uint32>(MAX_TEXTURE_BINDINGS - 1)){
		Reporter::report("Cannot bind texture. Chosen texture unit is too large.", Reporter::ERROR);
		return false;
	}

	ensureContext();

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glBindSampler(unit, _samplerId);

	_currentTexture2DBinding[unit] = _textureId;

	return true;
}

const GLuint& Texture::getId() const {
	return _textureId;
}

/////////////////////////////////////////////////////////////////////

bool Texture::loadFromFile(const std::string& file) {

	ensureContext();

	_textureId = SOIL_load_OGL_texture(	file.c_str(),
										SOIL_LOAD_AUTO,
										_textureId,
										SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
										| SOIL_FLAG_COMPRESS_TO_DXT);

	setFiltering(MAG_BILINEAR, MIN_TRILINEAR);

	return (_textureId != 0);
}

} /* namespace burn */
