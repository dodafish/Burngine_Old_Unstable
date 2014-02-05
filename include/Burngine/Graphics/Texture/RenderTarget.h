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

#ifndef RENDERTARGET_H_
#define RENDERTARGET_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>

#include <Burngine/System/NonCopyable.h>
#include <Burngine/System/Math.h>

#include <vector>

namespace burn {

class Texture;

class BURNGINE_API RenderTarget : public NonCopyable {
public:
	RenderTarget();
	~RenderTarget();

	enum DepthbufferType {
		NO_DEPTHBUFFER,
		DEPTHBUFFER_16 = GL_DEPTH_COMPONENT16,
		DEPTHBUFFER_24 = GL_DEPTH_COMPONENT24,
		DEPTHBUFFER_32 = GL_DEPTH_COMPONENT32
	};

	bool create(const Vector2ui& dimensions,
				const DepthbufferType& depthbufferType,
				const Texture& texture);

	bool addColorAttachment(const Texture& texture,
							const GLuint& attachmentPosition);
	bool removeColorAttachment(const Texture& texture);

	bool bind() const;

	void clear() const;

private:

	struct ColorAttachment {
		ColorAttachment(const GLuint& textureIdArg,
						const GLuint& attachmentPositionArg);

		GLuint textureId;
		GLuint attachmentPosition;
	};

	std::vector<ColorAttachment> _colorAttachments;

	void cleanup();

	//std::vector<const Texture&> _attachedTextures;

	GLuint _framebuffer;
	GLuint _depthbuffer;

	Vector2ui _dimensions;

};

} /* namespace burn */
#endif /* RENDERTARGET_H_ */
