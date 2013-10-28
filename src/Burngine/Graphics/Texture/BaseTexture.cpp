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

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

BaseTexture::BaseTexture() {
	_sampler.create();
}

BaseTexture::~BaseTexture() {
	//Default D-Tor for virtual causes
}

bool BaseTexture::setFiltering(const Sampler::MagnificationFiltering& mag, const Sampler::MinificationFiltering& min){
	return _sampler.setFiltering(mag, min);
}

bool BaseTexture::setSamplerParameter(GLenum parameter, GLenum value){
	return _sampler.setSamplerParameter(parameter, value);
}

bool BaseTexture::setAnisotropicLevel(const GLfloat& level){
	return _sampler.setAnisotropicLevel(level);
}

void BaseTexture::bind(const unsigned int& unit) const {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to bind. No valid context created!", Reporter::ERROR);
		return;
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	_sampler.bind(unit);
	onBind(unit);

}

void BaseTexture::unbind(const unsigned int& unit) const {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to unbind. No valid context created!", Reporter::ERROR);
		return;
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	Sampler::unbind(unit);
	onUnbind(unit);

}

} /* namespace burn */
