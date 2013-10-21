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

BaseTexture::BaseTexture() :
_texture(0),
_sampler(0),
_mipmapsGenerated(false),
_magnificationFiltering(MAG_NEAREST),
_minificationFiltering(MIN_NEAREST),
_anisotropicLevel(1.f),
_unit(0),
_referenceCount(new unsigned int(1)) {
}

BaseTexture::BaseTexture(const BaseTexture& other) :
_texture(other._texture),
_sampler(other._sampler),
_mipmapsGenerated(other._mipmapsGenerated),
_magnificationFiltering(other._magnificationFiltering),
_minificationFiltering(other._minificationFiltering),
_anisotropicLevel(other._anisotropicLevel),
_unit(other._unit),
_referenceCount(other._referenceCount) {
	++(*_referenceCount);
}

BaseTexture& BaseTexture::operator=(const BaseTexture& other) {

	--(*_referenceCount);

	if(*_referenceCount == 0)
		cleanup();

	_texture = other._texture;
	_sampler = other._sampler;
	_mipmapsGenerated = other._mipmapsGenerated;
	_magnificationFiltering = other._magnificationFiltering;
	_minificationFiltering = other._minificationFiltering;
	_anisotropicLevel = other._anisotropicLevel;
	_unit = other._unit;
	_referenceCount = other._referenceCount;

	++(*_referenceCount);

	return *this;
}

BaseTexture::~BaseTexture() {
	--(*_referenceCount);

	if(*_referenceCount == 0)
		cleanup();
}

bool BaseTexture::create(const Vector2ui& dimensions) {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated()){
		Reporter::report("Unable to create texture (No valid OpenGL-Context)!", Reporter::ERROR);
		return false;
	}

	//Generate texture and sampler. Does cleanup before if needed
	generate();

	//Set values
	_originalDimensions = dimensions;
	//Real texture dimensions are power of 2
	calculateDimensions(_originalDimensions);

	return true;

}

void BaseTexture::calculateDimensions(const Vector2ui& dimensions) {
	_dimensions.x = nextPowerOf2(dimensions.x);
	_dimensions.y = nextPowerOf2(dimensions.y);
}

GLint BaseTexture::getCurrentBoundTexture() const {
	GLint t = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &t);
	return t;
}

void BaseTexture::bind() const {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	//Tell our filter
	updateFiltering();

	//If not created before, this will produce the same effect
	//as unbind()
	glActiveTexture(GL_TEXTURE0 + _unit);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glBindSampler(_unit, _sampler);

}

void BaseTexture::updateFiltering() const {

	if(!Window::isContextCreated() || !isCreated()){
		return;
	}

	// Set magnification filter
	if(_magnificationFiltering == MAG_NEAREST)
		glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if(_magnificationFiltering == MAG_BILINEAR)
		glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if(_minificationFiltering == MIN_NEAREST)
		glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(_minificationFiltering == MIN_BILINEAR)
		glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(_minificationFiltering == MIN_NEAREST_MIPMAP)
		glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(_minificationFiltering == MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if(_minificationFiltering == MIN_TRILINEAR)
		glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Set Anisotropic
	glSamplerParameteri(_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, _anisotropicLevel);

}

void BaseTexture::setSamplerParameter(GLenum parameter, GLenum value) {
	if(Window::isContextCreated())
		glSamplerParameteri(_sampler, parameter, value);
}

void BaseTexture::unbind() {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	glActiveTexture(GL_TEXTURE0 + _unit);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindSampler(_unit, 0);

}

Uint32 BaseTexture::nextPowerOf2(const Uint32& n) const {

	//The power of 2 value to return
	Uint32 p2 = 1;

	//Look for the next greater power of 2
	while(n > p2){
		p2 <<= 1; //Shift one to the left. Equals p2 *= 2
	}

	return p2;

}

bool BaseTexture::isCreated() const {
	return (_texture != 0);
}

void BaseTexture::setFiltering(const MagnificationFiltering& mag, const MinificationFiltering& min) {
	_magnificationFiltering = mag;
	_minificationFiltering = min;

	//Tell OpenGL our filtering
	updateFiltering();
}

void BaseTexture::generate() {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	//Reset before generating new texture, sampler etc.
	cleanup();

	//Generate
	glGenTextures(1, &_texture);
	glGenSamplers(1, &_sampler);

}

void BaseTexture::cleanup() {

	//Valid OpenGL-Context is needed
	if(!Window::isContextCreated())
		return;

	if(!isCreated())
		return;

	//Delete texture and sampler from GPU
	glDeleteTextures(1, &_texture);
	glDeleteSamplers(1, &_sampler);

	//Reset values
	_texture = 0;
	_sampler = 0;
	_mipmapsGenerated = false;

}

const Vector2ui& BaseTexture::getDimensions() const {
	return _dimensions;
}

const Vector2ui& BaseTexture::getOriginalDimensions() const {
	return _originalDimensions;
}

Vector2f BaseTexture::mapUvCoordsToTexture(const Vector2f& uv) const {
	return Vector2f(
	static_cast<float>(uv.x) * (static_cast<float>(_originalDimensions.x) / static_cast<float>(_dimensions.x)),
	static_cast<float>(uv.y) * (static_cast<float>(_originalDimensions.y) / static_cast<float>(_dimensions.y)));
}

void BaseTexture::setToUnit(const unsigned int& unit) {
	_unit = unit;
}

GLfloat BaseTexture::getMaxAnisotropicLevel() {
	GLfloat maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	return maxAniso;
}

void BaseTexture::setAnisotropicLevel(const GLfloat& level) {
	_anisotropicLevel = level;
}

const GLfloat& BaseTexture::getAnisotropicLevel() const {
	return _anisotropicLevel;
}

} /* namespace burn */
