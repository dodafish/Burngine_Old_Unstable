//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/General/OpenGL.h>

#include <Burngine/Graphics/General/OpenGlControl.h>

namespace burn {

CubeMap::CubeMap() :
_cubemap(0),
_referenceCount(new unsigned int(1)) {

}

CubeMap::CubeMap(const CubeMap& other) :
_cubemap(other._cubemap),
_referenceCount(other._referenceCount) {
	++(*_referenceCount);
}

CubeMap& CubeMap::operator=(const CubeMap& other) {

	if(this == &other)
		return *this;

	if(*_referenceCount < 2){
		cleanup();
		delete _referenceCount;
	}else{
		--(*_referenceCount);
	}

	_cubemap = other._cubemap;
	_referenceCount = other._referenceCount;

	++(*_referenceCount);

	return *this;
}

CubeMap::~CubeMap() {
	if(*_referenceCount < 2){
		cleanup();
		delete _referenceCount;
	}else{
		--(*_referenceCount);
	}
}

bool CubeMap::isCreated() const {
	return (_cubemap != 0);
}

bool CubeMap::loadFromFile(const std::string& filePositiveX, const std::string& fileNegativeX,
const std::string& filePositiveY, const std::string& fileNegativeY, const std::string& filePositiveZ,
const std::string& fileNegativeZ) {

	generate();

	SOIL_load_OGL_cubemap(
	filePositiveX.c_str(), fileNegativeX.c_str(), filePositiveY.c_str(), fileNegativeY.c_str(), filePositiveZ.c_str(),
	fileNegativeZ.c_str(), SOIL_LOAD_AUTO, _cubemap,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	//Avoid ugly edge-artifacts
	setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}

void CubeMap::onBind(const unsigned int& unit) const {

	if(_cubemap == 0){
		Reporter::report("Binding of uncreated CubeMap disallowed!", Reporter::WARNING);
		return;
	}

	OpenGlControl::bindCubeMap(_cubemap, unit);
}

void CubeMap::generate() {

	ensureContext();

	if(*_referenceCount < 2){
		cleanup();
	}else{
		--(*_referenceCount);
		_referenceCount = new unsigned int(1);
		_cubemap = 0;
	}

	glGenTextures(1, &_cubemap);

	if(_cubemap == 0){
		Reporter::report("CubeMap creation failed!", Reporter::ERROR);
	}

}

void CubeMap::cleanup() {

	if(_cubemap == 0)
		return;

	ensureContext();

	glDeleteTextures(1, &_cubemap);

	_cubemap = 0;

}

} /* namespace burn */
