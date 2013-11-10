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

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

namespace burn {

/**
 * @brief Describes the material of surfaces
 */
class BURNGINE_API Material {
public:
	/**
	 * @brief The default constructor
	 * Default values:
	 * - Type: SOLID_COLOR
	 * - Flag[LIGHTING]: false
	 */
	Material();

	enum Flag {
		LIGHTING = 0, ///< Can the surface be lit?

		COUNT //Keep last!
	};

	enum Type {
		SOLID_COLOR = 0, //Default
		TEXTURED
	};

	/**
	 * @brief Sets the specified flag on true or false
	 *
	 * @param flag The flag to set
	 * @param enabled Sets the specified flag on true or false
	 *
	 * @see isFlagSet()
	 */
	void setFlag(Flag flag, bool enabled = true);

	/**
	 * @brief Sets the type of the material. The SceneNode
	 * will choose the right Shader.
	 *
	 * @param type The materialtype.
	 *
	 * @see getType()
	 *
	 * @note Ensure that you have loaded the BurngineShaders before
	 * renering. E.g. by calling BurngineShaders::loadAllShaders()
	 */
	void setType(Type type);

	/**
	 * @brief Returns the current type
	 *
	 * @return The current materialtype
	 *
	 * @see setType()
	 */
	const Type& getType() const;

	/**
	 * @brief Returns the current status of a flag
	 *
	 * @param flag The flag to check
	 *
	 * @return Status of a flag.
	 *
	 * @see setFlag()
	 */
	bool isFlagSet(Flag flag) const;

	/**
	 * @brief Sets the specular color that should be reflected
	 * from the surface
	 *
	 * @param color The specular color
	 *
	 * @see getSpecularColor()
	 */
	void setSpecularColor(const Vector3f& color);

	/**
	 * @brief Returns the specular that the surface reflects
	 *
	 * @return The reflected specular color
	 *
	 * @see setSpecularColor()
	 */
	const Vector3f& getSpecularColor() const;

	/**
	 * @brief Sets the diffuse color of the surface
	 *
	 * @param color The diffuse color
	 *
	 * @see getDiffuseColor()
	 */
	void setDiffuseColor(const Vector3f& color);

	/**
	 * @brief Returns the diffuse color of the surface
	 *
	 * @return The diffuse color
	 *
	 * @see setDiffuseColor()
	 */
	const Vector3f& getDiffuseColor() const;

	/**
	 * @brief Internal use only. Will be removed in future versions!
	 */
	void setIndex(const unsigned int& index);

	/**
	 * @brief Internal use only. Will be removed in future versions!
	 */
	const unsigned int& getIndex() const;

	/**
	 * @brief Sets whether the surface uses the diffuse color or a
	 * texture.
	 *
	 * @param shouldUseDiffuse Set to false if surface uses a texture,
	 *  true otherwise
	 *
	 * @see isUsingDiffuseColor()
	 */
	void useDiffuseColor(bool shouldUseDiffuse = true);

	/**
	 * @brief Returns true if the surface has no texture and uses the
	 * diffuse color instead.
	 *
	 * @return Returns false if surface uses a texture, true otherwise
	 *
	 * @see useDiffuseColor()
	 */
	bool isUsingDiffuseColor() const;

private:
	Type _type;
	bool _flags[COUNT];

	Vector3f _specularColor, _diffuseColor;
	unsigned int _index;
	bool _useDiffuseColor;
};

} /* namespace burn */
#endif /* MATERIAL_H_ */
