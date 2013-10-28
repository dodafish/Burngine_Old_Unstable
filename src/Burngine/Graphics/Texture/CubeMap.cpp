/*
 * CubeMap.cpp
 *
 *  Created on: 28.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Texture/CubeMap.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>

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
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

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

	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap);

}

void CubeMap::onUnbind(const unsigned int& unit) const {

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CubeMap::generate() {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to create CubeMap. No valid context created!", Reporter::ERROR);
		return;
	}

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

	if(!Window::isContextCreated()){
		Reporter::report("Unable to cleanup CubeMap. No valid context created!", Reporter::ERROR);
		return;
	}

	glDeleteTextures(1, &_cubemap);

	_cubemap = 0;

}

} /* namespace burn */
