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

#ifndef SKYBOX_H_
#define SKYBOX_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Texture/CubeMap.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/General/VertexBufferObject.h>

namespace burn {

/**
 * @brief A set of six textures (cubemap) used for simulating
 * sky and horizon
 */
class BURNGINE_API SkyBox {
public:
	/**
	 * @brief Default constructor initializing some
	 * values
	 */
	SkyBox();

	/**
	 * @brief Copyconstructor makes a identical copy of the other
	 * SkyBox and holding its data seperatly
	 */
	SkyBox(const SkyBox& other);

	/**
	 * @brief Assigmentoperator makes a identical copy of the other
	 * SkyBox and holding its data seperatly
	 */
	SkyBox& operator=(const SkyBox& other);

	/**
	 * @brief Sets the CubeMap texture which describes the skybox
	 *
	 * @param cubeMap The CubeMap with the texture
	 *
	 * @see getCubeMap()
	 */
	void setCubeMap(const CubeMap& cubeMap);

	/**
	 * @brief Returns the currently used CubeMap that describes
	 * the SkyBox
	 *
	 * @return The currently used CubeMap
	 *
	 * @see setCubeMap()
	 */
	const CubeMap& getCubeMap() const;

	/**
	 * @brief This draws the cubemap.
	 *
	 * @note This function will be deprecated in future versions
	 */
	void draw(const Camera& camera) const;

private:
	bool _isCubeMapSet;
	CubeMap _cubeMap;
	VertexBufferObject _boxVbo;
};

} /* namespace burn */
#endif /* SKYBOX_H_ */
