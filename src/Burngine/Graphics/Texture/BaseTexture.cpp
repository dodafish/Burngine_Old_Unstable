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

#include <Burngine/Graphics/Texture/BaseTexture.h>

namespace burn {

Int32 BaseTexture::_realTextureBindingCap = 0;

BaseTexture::BaseTexture() :
_samplerId(0),
_magFilter(MAG_BILINEAR),
_minFilter(MIN_BILINEAR),
_dimensions(Vector2ui(0)),
_internalFormat(RGB) {

	ensureContext();
	glGenSamplers(1, &_samplerId);

	//Update Sampler
	setFiltering(_magFilter, _minFilter);

}

BaseTexture::~BaseTexture() {

	ensureContext();
	glDeleteSamplers(1, &_samplerId);

}

void BaseTexture::bindSampler(const Uint32& unit) const {

	ensureContext();
	glBindSampler(unit, _samplerId);

}

void BaseTexture::setFiltering(	const MagnificationFilter& mag,
								const MinificationFilter& min) const {

	ensureContext();

	_magFilter = mag;
	_minFilter = min;

	// Set magnification filter
	if(_magFilter == MAG_NEAREST)
		glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else
		//BILINEAR
		glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if(_minFilter == MIN_NEAREST)
		glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(_minFilter == MIN_BILINEAR)
		glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(_minFilter == MIN_NEAREST_MIPMAP)
		glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(_minFilter == MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else
		//TRILINEAR
		glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

}

const BaseTexture::MagnificationFilter& BaseTexture::getMagnificationFilter() const {
	return _magFilter;
}

const BaseTexture::MinificationFilter& BaseTexture::getMinificationFilter() const {
	return _minFilter;
}

void BaseTexture::setSamplerParameter(	const GLenum& parameter,
										const GLenum& value) {

	ensureContext();
	glSamplerParameteri(_samplerId, parameter, value);

}

GLenum BaseTexture::estimateFormat(const InternalFormat& internalFormat) const {

	if(internalFormat == DEPTH_COMPONENT || internalFormat == DEPTH_COMPONENT16 || internalFormat == DEPTH_COMPONENT24
	|| internalFormat == DEPTH_COMPONENT32 || internalFormat == DEPTH_COMPONENT32F){
		return GL_DEPTH_COMPONENT;
	}else if(internalFormat == DEPTH_STENCIL){
		return GL_DEPTH_STENCIL;
	}else if(internalFormat == RED){
		return GL_RED;
	}else if(internalFormat == RG || internalFormat == RG32F){
		return GL_RG;
	}

	//else:
	return GL_RGB;
}

GLenum BaseTexture::estimateType(const InternalFormat& internalFormat) const {

	if(internalFormat == RG32F || internalFormat == DEPTH_COMPONENT32F)
		return GL_FLOAT;

	//else:
	return GL_UNSIGNED_BYTE;

}

const Vector2ui& BaseTexture::getDimensions() const {
	return _dimensions;
}

const BaseTexture::InternalFormat& BaseTexture::getInternalFormat() const {
	return _internalFormat;
}

} /* namespace burn */
