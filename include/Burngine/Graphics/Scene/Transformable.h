/*
 * Transformable.h
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

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
	 * @brief The default constructor
	 */
	Transformable();

	Transformable(const Transformable& other);

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
