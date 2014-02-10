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

#include <Burngine/Graphics/Texture/CubeMap.h>
#include <Burngine/System/Reporter.h>

#include <Burngine/extern/SOIL.h>

namespace burn {

void CubeMap::ensureConstants() {
	if(_realTextureBindingCap == 0){
		//Get OpenGL informations about textures
		ensureContext();
		for(int i = 0; i != MAX_TEXTURE_BINDINGS; ++i)
			_currentCubeMapBinding[i] = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_realTextureBindingCap);

		if(_realTextureBindingCap == 0){
			Reporter::report("Could not estimate maximum number of texture units!", Reporter::ERROR);
			exit(1);    //Kill process
		}

	}
}

GLuint CubeMap::_currentCubeMapBinding[];

CubeMap::CubeMap() :
_cubemapId(0) {
	ensureConstants();
}

CubeMap::CubeMap(	const Vector2ui& dimensions,
					const InternalFormat& internalFormat) :
_cubemapId(0) {

	ensureConstants();

	//Create texture immediatly
	create(_dimensions, _internalFormat);

}

CubeMap::~CubeMap() {
	cleanup();
}

void CubeMap::cleanup() {

	if(_cubemapId != 0){

		//Unbind where necessary
		for(size_t i = 0; i != MAX_TEXTURE_BINDINGS; ++i){
			if(_currentCubeMapBinding[i] == _cubemapId){
				_currentCubeMapBinding[i] = 0;
			}
		}

		ensureContext();
		glDeleteTextures(1, &_cubemapId);

		_cubemapId = 0;
	}

}

bool CubeMap::create(	const Vector2ui& dimensions,
						const InternalFormat& internalFormat) {

	if(dimensions.x != dimensions.y || dimensions.x == 0 || dimensions.y == 0){
		Reporter::report("Unable to create cubemap. Dimensions are invalid!", Reporter::ERROR);
		return false;
	}

	_dimensions = dimensions;
	_internalFormat = internalFormat;

	ensureContext();

	//Remove old texture and delete opengl entry
	cleanup();
	//Generate texture
	glGenTextures(1, &_cubemapId);

	//Assign memory on gpu
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapId);

	for(int i = 0; i != 6; ++i)
		glTexImage2D( 	GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0,
						_internalFormat,
						_dimensions.x,
						_dimensions.y,
						0,
						estimateFormat(_internalFormat),
						estimateType(_internalFormat),
						0);

	setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Restore previous bound texture
	glBindTexture(GL_TEXTURE_2D, _currentCubeMapBinding[0]);

	if(_cubemapId == 0){
		Reporter::report("Failed to create cubemap.", Reporter::ERROR);
	}

	return (_cubemapId != 0);
}

bool CubeMap::bind(const Uint32& unit) const {

	if(_cubemapId == 0){
		Reporter::report("Cannot bind cubemap. Cubemap has not been created.", Reporter::ERROR);
		return false;
	}
	if(unit > _realTextureBindingCap - 1 || unit > MAX_TEXTURE_BINDINGS - 1){
		Reporter::report("Cannot bind cubemap. Chosen texture unit is too large.", Reporter::ERROR);
		return false;
	}

	ensureContext();

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapId);
	glBindSampler(unit, _samplerId);

	_currentCubeMapBinding[unit] = _cubemapId;

	return true;
}

const GLuint& CubeMap::getId() const {
	return _cubemapId;
}

/////////////////////////////////////////////////////////////////////

bool CubeMap::loadFromFile(const std::string& filePositiveX, const std::string& fileNegativeX,
const std::string& filePositiveY, const std::string& fileNegativeY, const std::string& filePositiveZ,
const std::string& fileNegativeZ) {

	ensureContext();

	_cubemapId = SOIL_load_OGL_cubemap(
	filePositiveX.c_str(), fileNegativeX.c_str(), filePositiveY.c_str(), fileNegativeY.c_str(), filePositiveZ.c_str(),
	fileNegativeZ.c_str(), SOIL_LOAD_AUTO, _cubemapId,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	//Avoid ugly edge-artifacts
	setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return (_cubemapId != 0);
}

} /* namespace burn */
