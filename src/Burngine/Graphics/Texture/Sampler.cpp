/*
 * Sampler.cpp
 *
 *  Created on: 26.10.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Texture/Sampler.h>
#include <Burngine/Graphics/Window/Window.h>

namespace burn {

void bind(const Sampler& sampler, const unsigned int& unit) {
	if(!Window::isContextCreated() || !sampler.isCreated())
		return;
	glBindSampler(unit, sampler._id);
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

Sampler::Sampler() :
_id(0),
_referenceCounter(new unsigned int(1)) {
}

Sampler::Sampler(const Sampler& other) :
_id(other._id),
_referenceCounter(other._referenceCounter) {
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

	if(!Window::isContextCreated())
		return false;

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

	if(!Window::isContextCreated() || _id == 0)
		return;

	glDeleteSamplers(1, &_id);

	_id = 0;
}

bool Sampler::isCreated() const {
	return (_id != 0);
}

} /* namespace burn */
