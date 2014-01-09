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

#ifndef GBUFFER_H_
#define GBUFFER_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>

namespace burn {

/**
 * @brief The GBuffer is used for deferred shading.
 */
class BURNGINE_API GBuffer {
public:
	/**
	 * @brief Default constructor initializing some
	 * values
	 */
	GBuffer();

	/**
	 * @brief Default destructor cleaning up GBuffer
	 */
	~GBuffer();

	//GBuffer is not copyable!
	GBuffer(const GBuffer& other) = delete;
	GBuffer& operator=(const GBuffer& other) = delete;

	/**
	 * @brief Creates the GBuffer on the GPU to be ready
	 * to get geometrical buffers passed
	 *
	 * @param dimensions The dimensions of the buffers
	 */
	bool create(const Vector2ui& dimensions);

	/**
	 * @brief Clears the buffers. I.e. clears the textures and
	 * clears the buffer bits of the framebuffer
	 */
	void clear();

	/**
	 * @brief Binds the GBuffer as target enabling passing
	 * geometrical buffers to GBuffer
	 *
	 * @see bindAsSource()
	 */
	void bindAsTarget() const;

	/**
	 * @brief Binds the GBuffer as source with an attachment
	 * offset enabling sampling from the geometrical buffers
	 *
	 * @param offset Offset value from GL_TEXTURE0
	 *
	 * @see bindAsTarget()
	 */
	void bindAsSource(const unsigned int& offset = 0) const;

	enum GBufferType {
		DIFFUSE = 0, ///< Diffuse colors
		NORMAL_WS, ///< Normals in worldspace
		POSITION_WS, ///< Positions in worldspace

		COUNT ///< Geometrical buffer count; Keep last
	};

	/**
	 * @brief Sets a buffer as source framebuffer. Used for copying
	 * whole buffers to another.
	 *
	 * @param buffer The buffer to set as source
	 */
	void setSourceBuffer(const GBufferType& buffer);

	/**
	 * @brief Binds the depthattachment as texture to texture
	 * unit 0
	 */
	void bindDepthBufferAsSourceTexture() const;

	/**
	 * @brief Return the dimensions of the GBuffer
	 *
	 * @return Dimensions of the GBuffer
	 */
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
