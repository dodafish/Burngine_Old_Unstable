/*
 * Material.h
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../Export.h"
#include "../System/Math.h"

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

private:
	Type _type;
	bool _flags[COUNT];

	Vector3f _specularColor;
};

} /* namespace burn */
#endif /* MATERIAL_H_ */
