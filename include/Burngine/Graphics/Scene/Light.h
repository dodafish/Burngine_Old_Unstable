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

#ifndef LIGHT_H_
#define LIGHT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Scene/Transformable.h>
#include <Burngine/Graphics/Scene/Scene.h>

namespace burn {

/**
 * @brief Lightsource with the behaviour of a pointlight
 */
class BURNGINE_API Light : public Transformable {
public:
	/**
	 * @brief Default constructor initializing some
	 * values
	 */
	Light();

	/**
	 * @brief CopyContructor copying the attributes of
	 * the other light and attaching itself to the same
	 * Scenes of the other one
	 *
	 * @param other The other light to copy from
	 */
	Light(const Light& other);

	/**
	 * @brief CopyContructor copying the attributes of
	 * the other light and attaching itself to the same
	 * Scenes of the other one.
	 *
	 * @param other The other light to copy from
	 *
	 * @note You don't have to care about detaching this light
	 * before copying from another. The method will do that for you
	 * automatically
	 */
	Light& operator=(const Light& other);

	/**
	 * @brief Default destructor detaching this light from all
	 * attached Scenes
	 */
	virtual ~Light();

	/**
	 * @brief Sets the color of the lightsource
	 *
	 * @param color The color the light shall have
	 *
	 * @see getColor()
	 */
	void setColor(const Vector3f& color);

	/**
	 * @brief Returns the current color of the lightsource
	 *
	 * @return The current color of the lightsource
	 *
	 * @see setColor()
	 */
	const Vector3f& getColor() const;

	/**
	 * @brief Sets the intensity of the lightsource. The higher
	 * the brighter.
	 *
	 * @param intensity The intensity the light shall have
	 *
	 * @see getIntensity()
	 */
	void setIntensity(const float& intensity);

	/**
	 * @brief Returns the current intensity of the lightsource
	 *
	 * @return The current intensity of the lightsource
	 *
	 * @see setIntensity()
	 */
	const float& getIntensity() const;

	/**
	 * @brief Adds a Scene as parent. This is used by Scene when
	 * attaching Lights. You should use the Scene's methods for
	 * connecting Lights to Scenes instead of this method
	 *
	 * @param parent The parent Scene
	 *
	 * @see removeParentScene()
	 */
	void addParentScene(Scene* scene);

	/**
	 * @brief Removes a Scene from parents. This is used by Scene when
	 * detaching Lights. You should use the Scene's methods for
	 * disconnecting Lights from Scenes instead of this method
	 *
	 * @param parent The parent Scene
	 *
	 * @see addParentScene()
	 */
	void removeParentScene(Scene* scene);

	/**
	 * @brief Softness is in development. Better don't use this
	 * (set it to 0.f; default) until further notice
	 */
	void setSoftness(const float& softness);

	/**
	 * @brief Softness is in development. Better don't use this
	 * (set it to 0.f; default) until further notice
	 */
	const float& getSoftness() const;

protected:

	Vector3f _color;
	float _intensity;
	float _softness;

	std::vector<Scene*> _parents;
	void removeAllParents();
};

} /* namespace burn */
#endif /* LIGHT_H_ */
