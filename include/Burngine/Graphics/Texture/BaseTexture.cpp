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

BaseTexture::BaseTexture() :
_dimensions(Vector2ui(0)),
_internalFormat(RGB) {
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

} /* namespace burn */
