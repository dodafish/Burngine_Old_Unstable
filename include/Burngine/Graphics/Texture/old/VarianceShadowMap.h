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

#ifndef VARIANCESHADOWMAP_H_
#define VARIANCESHADOWMAP_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

namespace burn {

class BURNGINE_API VarianceShadowMap : public BaseTexture {
public:
	VarianceShadowMap();
	~VarianceShadowMap();

	//Non-copyable
	VarianceShadowMap(const VarianceShadowMap& other) = delete;
	VarianceShadowMap& operator=(const VarianceShadowMap& other) = delete;

	/**
	 * @brief Creates the rendertarget according to the given dimensions
	 *
	 * @param dimensions The dimensions the rendertexture should have
	 *
	 * @return Return true on success. False otherwise
	 */
	bool create(const Vector2ui& dimensions);

	/**
	 * @brief Binds the RenderTexture as rendertarget. All sequential
	 * rendercalls will get stored in this RenderTexture if no other
	 * RenderTarget has been bound
	 *
	 * @see clear()
	 */
	void bindAsTarget(bool bindForMsaaRendering = false) const;

	void finishMultisampling() const ;

	/**
	 * @brief Clears the RenderTexture.
	 */
	void clear() const;

	/**
	 * @brief Returns true if the RenderTexture has beed created
	 *
	 * @return Returns false if the RenderTexture has not been created yet
	 *
	 * @see create()
	 */
	bool isCreated() const;

	const Vector2ui& getDimensions() const;

private:

	virtual void onBind(const unsigned int& unit) const;

	void cleanup();

	GLuint _framebuffer, _msaaFramebuffer, _depthbuffer, _msaaDepthbuffer, _texture, _msaaTexture;
	bool _isCreated;
	Vector2ui _dimensions;

};

} /* namespace burn */
#endif /* VARIANCESHADOWMAP_H_ */
