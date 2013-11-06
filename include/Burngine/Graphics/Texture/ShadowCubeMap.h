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

#ifndef SHADOWCUBEMAP_H_
#define SHADOWCUBEMAP_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/BaseTexture.h>

namespace burn {

class BURNGINE_API ShadowCubeMap : public BaseTexture {
public:
	ShadowCubeMap();

	//Rendertargets are not copyable!
	ShadowCubeMap(const ShadowCubeMap& other) = delete;
	ShadowCubeMap& operator=(const ShadowCubeMap& other) = delete;

	~ShadowCubeMap();

	enum Resolution {
		VERY_LOW = 128, ///< 128x128
		LOW = 256, ///< 256x256
		MEDIUM = 512, ///< 512x512
		HIGH = 1024, ///< 1024x1024
		VERY_HIGH = 2048 ///< 2048x2048
	};


	bool create(const Resolution& resolution = HIGH);

	void clear();

	void bindAsRendertarget(const int& face) const;

	bool isCreated() const;

	const Resolution& getResolution() const;

private:
	virtual void onBind(const unsigned int& unit) const;
	virtual void onUnbind(const unsigned int& unit) const;

	void cleanup();

	GLuint _framebuffer, _cubeMap;
	Resolution _resolution;
	bool _isCreated;
};

} /* namespace burn */
#endif /* SHADOWCUBEMAP_H_ */
