/*
 * Transformable.h
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#ifndef TRANSFORMABLE_H_
#define TRANSFORMABLE_H_

#include "../Export.h"
#include "../System/Math.h"

namespace burn {

class BURNGINE_API Transformable {
public:
	Transformable();
	virtual ~Transformable();

	void setPosition(const Vector3f& position);
	const Vector3f& getPosition() const;
	void setRotation(const Vector3f& rotation);
	const Vector3f& getRotation() const;
	void setScale(const Vector3f& scale);
	const Vector3f& getScale() const;

	const Matrix4f& getModelMatrix();

protected:

	Vector3f _position, _scale, _rotation;
	Matrix4f _modelMatrix;

};

} /* namespace burn */
#endif /* TRANSFORMABLE_H_ */
