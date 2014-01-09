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

#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/DirectionalLight.h>

namespace burn {

/**
 * @brief Lightsource imitating a light that shines in the shape of
 * a cone. E.g. a flashlight
 */
class BURNGINE_API SpotLight : public DirectionalLight {
public:
	/**
	 * @brief Default constructor initialzing the coneangle with a value
	 * of 30 degrees
	 */
	SpotLight();

	/**
	 * @brief Sets the cone angle of the spotlight. Surfaces outside the
	 * cone will not be lit.
	 *
	 * @param angle The coneangle in degrees
	 *
	 * @see getConeAngle()
	 */
	void setConeAngle(const float& angle);

	/**
	 * @brief Returns the coneangle in degrees.
	 *
	 * @return The coneangle in degrees
	 *
	 * @see setConeAngle()
	 */
	const float& getConeAngle() const;

private:
	float _coneAngle;

};

} /* namespace burn */
#endif /* SPOTLIGHT_H_ */
