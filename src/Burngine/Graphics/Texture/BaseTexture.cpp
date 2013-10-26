/*
 * BaseTexture.cpp
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Texture/BaseTexture.h>
#include <Burngine/Graphics/Window/Window.h>

#include <Burngine/System/Reporter.h>
#include <sstream>

namespace burn {

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

Uint32 nextPowerOf2(const Uint32& n) {

	//The power of 2 value to return
	Uint32 p2 = 1;

	//Look for the next greater power of 2
	while(n > p2){
		p2 <<= 1; //Shift one to the left. Equals p2 *= 2
	}

	return p2;

}

Vector2ui BaseTexture::calculatePow2Dimensions(const Vector2ui& dimensions) {
	Vector2ui dim;
	dim.x = nextPowerOf2(dimensions.x);
	dim.y = nextPowerOf2(dimensions.y);
	return dim;
}

GLfloat BaseTexture::getMaxAnisotropicLevel() {
	GLfloat maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	return maxAniso;
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

BaseTexture::BaseTexture() :
_sampler(0),
_referenceCount(new unsigned int(1)),
_magnificationFiltering(MAG_NEAREST),
_minificationFiltering(MIN_NEAREST),
_anisotropicLevel(1.f) {
}

BaseTexture::BaseTexture(const BaseTexture& other) :
_sampler(other._sampler),
_referenceCount(other._referenceCount),
_magnificationFiltering(other._magnificationFiltering),
_minificationFiltering(other._minificationFiltering),
_anisotropicLevel(other._anisotropicLevel) {
	++(*_referenceCount);
}

BaseTexture& BaseTexture::operator=(const BaseTexture& other) {

	if(*_referenceCount < 2){
		glDeleteSamplers(1, &_sampler);
		delete _referenceCount;
	}else{
		--(*_referenceCount);
	}

	_sampler = other._sampler;
	_magnificationFiltering = other._magnificationFiltering;
	_minificationFiltering = other._minificationFiltering;
	_anisotropicLevel = other._anisotropicLevel;
	_referenceCount = other._referenceCount;

	++(*_referenceCount);

	return *this;
}

BaseTexture::~BaseTexture() {
	if(*_referenceCount < 2){
		glDeleteSamplers(1, &_sampler);
		delete _referenceCount;
	}else{
		--(*_referenceCount);
	}
}

bool BaseTexture::createSampler(){

	if(*_referenceCount < 2){
		destroySampler();
	}

}

void BaseTexture::destroySampler(){

	if(!Window::isContextCreated() && _sampler != 0)
		return;

	glDeleteSamplers(1, &_sampler);

}


} /* namespace burn */
