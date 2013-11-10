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

#ifndef TRANSFORMABLE_H_
#define TRANSFORMABLE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

namespace burn {

/**
 * @brief Provides methods to move an object in 3D-space
 */
class BURNGINE_API Transformable {
public:
	/**
	 * @brief The default constructor initializing
	 * modifiers
	 */
	Transformable();

	/**
	 * @brief Copyconstructor copies another Transformable's
	 * modifiers
	 */
	Transformable(const Transformable& other);

	/**
	 * @brief Assignmentoperator copies another Transformable's
	 * modifiers
	 */
	Transformable& operator=(const Transformable& other);

	/**
	 * @brief Virtual destructor, because this class should
	 * be derived only.
	 */
	virtual ~Transformable();

	/**
	 * @brief Sets the position of the object
	 *
	 * @param position The new position
	 *
	 * @see getPosition()
	 */
	void setPosition(const Vector3f& position);

	/**
	 * @brief Returns the current position of the object
	 *
	 * @return The current position
	 *
	 * @see setPosition()
	 */
	const Vector3f& getPosition() const;

	/**
	 * @brief Sets the rotation of the object
	 *
	 * @param rotation The new rotation
	 *
	 * @see getRotation()
	 */
	void setRotation(const Vector3f& rotation);

	/**
	 * @brief Returns the current rotation of the object
	 *
	 * @return The current rotation
	 *
	 * @see setRotation()
	 */
	const Vector3f& getRotation() const;

	/**
	 * @brief Sets the scale of the object
	 *
	 * @param scale The new scale
	 *
	 * @see getScale()
	 */
	void setScale(const Vector3f& scale);

	/**
	 * @brief Return the current scale of the object
	 *
	 * @return The current scale
	 *
	 * @see setScale()
	 */
	const Vector3f& getScale() const;

	/**
	 * @brief Returns the current modelmatrix of the object
	 *
	 * @return The current modelmatrix
	 */
	const Matrix4f& getModelMatrix();

protected:

	Vector3f _position, _scale, _rotation;
	Matrix4f _modelMatrix;

};

} /* namespace burn */
#endif /* TRANSFORMABLE_H_ */
