/*
 * Transformable.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Transformable.h>

namespace burn {

Transformable::Transformable() :
_position(Vector3f(0.f, 0.f, 0.f)),
_scale(Vector3f(1.f, 1.f, 1.f)),
_rotation(Vector3f(0.f, 0.f, 0.f)) {

}

Transformable::Transformable(const Transformable& other):
_position(other._position),
_scale(other._scale),
_rotation(other._rotation){
}

Transformable& Transformable::operator=(const Transformable& other) {

	if(this == &other)
		return *this;

	_position = other._position;
	_scale = other._scale;
	_rotation = other._rotation;

	return *this;

}

Transformable::~Transformable() {
}

void Transformable::setPosition(const Vector3f& position) {
	_position = position;
}

const Vector3f& Transformable::getPosition() const {
	return _position;
}

void Transformable::setRotation(const Vector3f& rotation) {
	_rotation = rotation;
}

const Vector3f& Transformable::getRotation() const {
	return _rotation;
}

void Transformable::setScale(const Vector3f& scale) {
	_scale = scale;
}

const Vector3f& Transformable::getScale() const {
	return _scale;
}

const Matrix4f& Transformable::getModelMatrix() {

	_modelMatrix = glm::translate(_position.x, _position.y, _position.z);
	_modelMatrix = glm::scale(_modelMatrix, _scale.x, _scale.y, _scale.z);
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.x, Vector3f(1.f, 0.f, 0.f));
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, Vector3f(0.f, 1.f, 0.f));
	_modelMatrix = glm::rotate(_modelMatrix, _rotation.z, Vector3f(0.f, 0.f, 1.f));
	return _modelMatrix;

}

} /* namespace burn */
