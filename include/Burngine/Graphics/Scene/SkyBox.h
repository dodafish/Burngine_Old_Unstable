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

#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/CubeMap.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

namespace burn {

/**
 * @brief A set of six textures (cubemap) used for simulating
 * sky and horizon
 */
class BURNGINE_API SkyBox {
public:
	SkyBox();
	SkyBox(const SkyBox& other);
	SkyBox& operator=(const SkyBox& other);

	void setCubeMap(const CubeMap& cubeMap);
	const CubeMap& getCubeMap() const;

	void draw();

private:
	CubeMap _cubeMap;
	VertexBufferObject _boxVbo;
};

} /* namespace burn */
#endif /* SKYBOX_H_ */
