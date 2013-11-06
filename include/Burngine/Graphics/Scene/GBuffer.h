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

#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>

namespace burn {

class BURNGINE_API GBuffer {
public:
	GBuffer();
	~GBuffer();

	//GBuffer is not copyable!
	GBuffer(const GBuffer& other) = delete;
	GBuffer& operator=(const GBuffer& other) = delete;

	bool create(const Vector2ui& dimensions);

	void clear();

	void bindAsTarget() const;
	void bindAsSource(const unsigned int& offset = 0) const;

	enum GBufferType {
		DIFFUSE = 0, NORMAL_WS, POSITION_WS,

		COUNT
	};

	void setSourceBuffer(const GBufferType& buffer);
	void bindDepthBufferAsSourceTexture() const;

	const Vector2ui& getDimensions() const;

private:

	bool _isCreated;
	GLuint _textures[COUNT];
	GLuint _depthTexture;
	GLuint _framebuffer;
	Vector2ui _dimensions;
};

} /* namespace burn */
#endif /* GBUFFER_H_ */
