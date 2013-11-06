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

class BURNGINE_API Material {
public:
	/**
	 * @brief The default constructor
	 * Default values:
	 * - Type: SOLID_COLOR
	 * - Flag[LIGHTING]: false
	 */
	Material();

	/**
	 * @brief The default destructor
	 */
	~Material();

	enum Flag {
		LIGHTING = 0,

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

	void setSpecularColor(const Vector3f& color);
	const Vector3f& getSpecularColor() const;

	void setDiffuseColor(const Vector3f& color);
	const Vector3f& getDiffuseColor() const;

	void setIndex(const unsigned int& index);
	const unsigned int& getIndex() const;

	void useDiffuseColor(bool shouldUseDiffuse = true);
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
