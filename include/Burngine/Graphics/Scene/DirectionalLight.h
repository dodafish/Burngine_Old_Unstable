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

#ifndef DIRECTIONAL_LIGHT_H_
#define DIRECTIONAL_LIGHT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Light.h>

namespace burn {

/**
 * @brief Fakes lightsources that are far away with (almost)
 * parallel lightrays (e.g. the sun)
 */
class BURNGINE_API DirectionalLight : public Light {
public:

	/**
	 * @brief Returns the direction the light is facing.
	 *
	 * @return The direction of the light
	 */
	Vector4f getDirection() const;

};

} /* namespace burn */
#endif /* DIRECTIONAL_LIGHT_H_ */
