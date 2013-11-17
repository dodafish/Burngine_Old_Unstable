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

#include <Burngine/Graphics/Texture/Sampler.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/General/OpenGL.h>

namespace burn {

void Sampler::unbind(const unsigned int& unit) {
	ensureContext();

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

void Sampler::bind(const unsigned int& unit) const {

	if(!isCreated()){
		Reporter::report("Unable to bind sampler. Sampler is not created!", Reporter::ERROR);
		return;
	}

	ensureContext();

	if(_needsFilteringUpdate){
		if(!updateFiltering())
			Reporter::report("Unable to update sampler's filtering!", Reporter::ERROR);
	}

	glBindSampler(unit, _id);

}

bool Sampler::create() {

	ensureContext();

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

	ensureContext();

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

	if(!isCreated()){
		Reporter::report("Unable to update sampler's filtering. Sampler not created!", Reporter::ERROR);
		return false;
	}

	ensureContext();

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

bool Sampler::setSamplerParameter(	GLenum parameter,
									GLenum value) {

	if(isCreated()){
		ensureContext();

		glSamplerParameteri(_id, parameter, value);
		return true;
	}
	Reporter::report(	"Unable to set sampler's parameter. No valid context available or sampelr not created!",
						Reporter::ERROR);
	return false;
}

bool Sampler::setFiltering(	const MagnificationFiltering& mag,
							const MinificationFiltering& min) {
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
