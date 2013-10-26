/*
 * Sampler.cpp
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Texture/Sampler.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>

namespace burn {

void bind(const Sampler& sampler, const unsigned int& unit) {
	if(!Window::isContextCreated() || !sampler.isCreated()){
		Reporter::report("Unable to bind sampler. No valid context or sampler is not created!", Reporter::ERROR);
		return;
	}

	if(sampler._needsFilteringUpdate){
		if(!sampler.updateFiltering())
			Reporter::report("Unable to update sampler's filtering!", Reporter::ERROR);
	}

	glBindSampler(unit, sampler._id);
}

void unbind(const unsigned int& unit) {
	if(!Window::isContextCreated()){
		Reporter::report("Unable to unbind sampler. No valid context created!", Reporter::ERROR);
		return;
	}

	glBindSampler(unit, 0);
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

Sampler::Sampler() :
_id(0),
_referenceCounter(new unsigned int(1)),
_magnificationFiltering(MAG_NEAREST),
_minificationFiltering(MIN_NEAREST),
_anisotropicLevel(1.f),
_needsFilteringUpdate(true) {
}

Sampler::Sampler(const Sampler& other) :
_id(other._id),
_referenceCounter(other._referenceCounter),
_magnificationFiltering(other._magnificationFiltering),
_minificationFiltering(other._minificationFiltering),
_anisotropicLevel(other._anisotropicLevel),
_needsFilteringUpdate(other._needsFilteringUpdate) {
	++(*_referenceCounter);
}

Sampler& Sampler::operator=(const Sampler& other) {

	//Avoid selfassignment
	if(this == &other)
		return *this;

	if(*_referenceCounter < 2){
		destroy();
		delete _referenceCounter;
	}else{
		--(*_referenceCounter);
	}

	_id = other._id;
	_referenceCounter = other._referenceCounter;
	_magnificationFiltering = other._magnificationFiltering;
	_minificationFiltering = other._minificationFiltering;
	_anisotropicLevel = other._anisotropicLevel;
	_needsFilteringUpdate = other._needsFilteringUpdate;

	++(*_referenceCounter);

	return *this;
}

Sampler::~Sampler() {
	if(*_referenceCounter < 2){
		destroy();
		delete _referenceCounter;
	}else{
		--(*_referenceCounter);
	}
}

bool Sampler::create() {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to create sampler. No valid context available!", Reporter::ERROR);
		return false;
	}

	if(*_referenceCounter < 2){
		destroy();
	}else{
		--(*_referenceCounter);
		_referenceCounter = new unsigned int(1);
	}

	glGenSamplers(1, &_id);

	return true;
}

void Sampler::destroy() {

	if(!Window::isContextCreated()){
		Reporter::report("Unable to destroy sampler. No valid context available!", Reporter::ERROR);
		return;
	}

	if(!isCreated())
		return;

	glDeleteSamplers(1, &_id);

	//Reset values
	_id = 0;
	_magnificationFiltering = MAG_NEAREST;
	_minificationFiltering = MIN_NEAREST;
	_anisotropicLevel = 1.f;
	_needsFilteringUpdate = false;
}

bool Sampler::isCreated() const {
	return (_id != 0);
}

bool Sampler::updateFiltering() const {

	if(!Window::isContextCreated() || !isCreated()){
		Reporter::report("Unable to update sampler's filtering. No valid context available or sampler not created!",
							Reporter::ERROR);
		return false;
	}

	// Set magnification filter
	if(_magnificationFiltering == MAG_NEAREST)
		glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if(_magnificationFiltering == MAG_BILINEAR)
		glSamplerParameteri(_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if(_minificationFiltering == MIN_NEAREST)
		glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(_minificationFiltering == MIN_BILINEAR)
		glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(_minificationFiltering == MIN_NEAREST_MIPMAP)
		glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(_minificationFiltering == MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if(_minificationFiltering == MIN_TRILINEAR)
		glSamplerParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Set Anisotropic
	glSamplerParameteri(_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, _anisotropicLevel);

	_needsFilteringUpdate = false;

	return true;
}

bool Sampler::setSamplerParameter(GLenum parameter, GLenum value) {
	if(Window::isContextCreated() && isCreated()){
		glSamplerParameteri(_id, parameter, value);
		return true;
	}
	Reporter::report("Unable to set sampler's parameter. No valid context available or sampelr not created!",
						Reporter::ERROR);
	return false;
}

bool Sampler::setFiltering(const MagnificationFiltering& mag, const MinificationFiltering& min) {
	_magnificationFiltering = mag;
	_minificationFiltering = min;

	return updateFiltering();
}

bool Sampler::setAnisotropicLevel(const GLfloat& level) {
	_anisotropicLevel = level;
	return updateFiltering();
}

const GLfloat& Sampler::getAnisotropicLevel() const {
	return _anisotropicLevel;
}

} /* namespace burn */
