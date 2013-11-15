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
	 * @brief The default constructor initialzing default Flags
	 */
	Material();

	/**
	 * @brief Copies all flags and the type from another material
	 */
	Material(const Material& other);

	/**
	 * @brief Copies all flags and the type from another material
	 */
	Material& operator=(const Material& other);

	enum Flag {
		VERTEX_ORDER_CLOCKWISE = 0, ///< Are the vertices ordered clockwise or counter-clockwise? Default: false
		DRAW_Z_BUFFER, ///< Does the node draw into the depth buffer? Default: true
		CAST_SHADOWS, ///< Does the node cast shadows? Default: true

		COUNT //Keep last!
	};

	enum Type {
		SOLID_COLOR = 0, ///< Node has no texture, but colors.
		TEXTURED ///< Node has a single texture supposed being the diffuse one
	};

	/**
	 * @brief Sets the specified flag on true or false
	 *
	 * @param flag The flag to set
	 * @param enabled Sets the specified flag on true or false
	 *
	 * @see isFlagSet()
	 */
	void setFlag(	Flag flag,
					bool enabled = true);

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

private:
	Type _type;
	bool _flags[COUNT];

	Vector3f _specularColor, _diffuseColor;
};

} /* namespace burn */
#endif /* MATERIAL_H_ */
