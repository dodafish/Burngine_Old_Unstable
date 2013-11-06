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

#ifndef RENDERTEXTURE_H_
#define RENDERTEXTURE_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <Burngine/System/Math.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>
#include <vector>

namespace burn {

class BURNGINE_API RenderTexture : public BaseTexture{
public:
	RenderTexture();
	~RenderTexture();

	//Rendertargets are not copyable
	RenderTexture(const RenderTexture& other) = delete;
	RenderTexture& operator=(const RenderTexture& other) = delete;

	bool create(const Vector2ui& dimensions);

	void bindAsTarget() const;

	void clear() const;

	void drawFullscreen();
	void draw(const Vector2f& position, const Vector2f& size);

	bool isCreated() const;

	bool addColorAttachment(const GLenum& attachment);

private:
	virtual void onBind(const unsigned int& unit) const;
	virtual void onUnbind(const unsigned int& unit) const;

	void cleanup();

	GLuint _framebuffer, _depthbuffer, _texture;
	bool _isCreated;
	Vector2ui _dimensions;
	std::vector<GLenum> _attachments;
	std::vector<GLuint> _additionalTextures;
};

} /* namespace burn */
#endif /* RENDERTEXTURE_H_ */
