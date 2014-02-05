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

#ifndef BASETEXTURE_H_
#define BASETEXTURE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>
#include <Burngine/System/NonCopyable.h>

namespace burn {

class BURNGINE_API BaseTexture : public NonCopyable {
public:

	enum InternalFormat {
		//Base internal formats:
		DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL,
		RED = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		//Sized internal formats:
		RGB8 = GL_RGB8,
		RGB16 = GL_RGB16,
		RG32F = GL_RG32F,
		DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
		DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
		DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,
		DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F
	};

	const Vector2ui& getDimensions() const;
	const InternalFormat& getInternalFormat() const;

	enum MagnificationFilter {
		MAG_NEAREST, MAG_BILINEAR
	};
	enum MinificationFilter {
		MIN_NEAREST, MIN_BILINEAR, MIN_NEAREST_MIPMAP, MIN_BILINEAR_MIPMAP, MIN_TRILINEAR
	};

	void setFiltering(	const MagnificationFilter& mag,
						const MinificationFilter& min);
	const MagnificationFilter& getMagnificationFilter() const;
	const MinificationFilter& getMinificationFilter() const;
	void setSamplerParameter(	const GLenum& parameter,
								const GLenum& value);

protected:

	//Only inherited classes can be created
	BaseTexture();
	~BaseTexture();

	GLuint _samplerId;
	void bindSampler(const Uint32& unit) const;

	//Sampler attributes
	MagnificationFilter _magFilter;
	MinificationFilter _minFilter;

	//Texture attributes
	Vector2ui _dimensions;
	InternalFormat _internalFormat;

	///////////////////////////////////////////////////////////////////////////

	//Used for creation
	GLenum estimateFormat(const InternalFormat& internalFormat) const;
	GLenum estimateType(const InternalFormat& internalFormat) const;

};

} /* namespace burn */
#endif /* BASETEXTURE_H_ */
