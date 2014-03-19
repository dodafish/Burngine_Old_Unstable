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

#include <Burngine/Graphics/Scene/Transformable.h>
#include <Burngine/System/RotationUtil.h>

namespace burn {

Transformable::Transformable() :
_position(Vector3f(0.f, 0.f, 0.f)),
_scale(Vector3f(1.f, 1.f, 1.f)),
_modelMatrix(Matrix4f(1.f)) {
	updateModelMatrix();
}

Transformable::Transformable(const Transformable& other) :
_position(other._position),
_scale(other._scale),
_rotation(other._rotation),
_modelMatrix(other._modelMatrix) {
}

Transformable& Transformable::operator=(const Transformable& other) {

	if(this == &other)
		return *this;

	_position = other._position;
	_scale = other._scale;
	_rotation = other._rotation;
	_modelMatrix = other._modelMatrix;

	return *this;

}

Transformable::~Transformable() {
}

void Transformable::setPosition(const Vector3f& position) {
	_position = position;
	updateModelMatrix();
}

const Vector3f& Transformable::getPosition() const {
	return _position;
}

void Transformable::setRotation(const Rotation& rotation) {
	_rotation = rotation;
	updateModelMatrix();
}

const Rotation& Transformable::getRotation() const {
	return _rotation;
}

void Transformable::lookAt(const Vector3f& point){
	_rotation = RotationUtil::RotationBetweenVectors(Vector3f(1.f, 0.f, 0.f), point - _position);
	updateModelMatrix();
}

/*void Transformable::rotate(const float& offsetX, const float& offsetY, const float& offsetZ){
 _rotation.x += offsetX;
 _rotation.y += offsetY;
 _rotation.z += offsetZ;
 }*/

void Transformable::setScale(const Vector3f& scale) {
	_scale = scale;
	updateModelMatrix();
}

const Vector3f& Transformable::getScale() const {
	return _scale;
}

const Matrix4f& Transformable::getModelMatrix() {
	return _modelMatrix;
}

void Transformable::updateModelMatrix() {
	Matrix4f translationMatrix = glm::translate(_position.x, _position.y, _position.z);
	Matrix4f scaleMatrix = glm::scale(_scale.x, _scale.y, _scale.z);
	_modelMatrix = translationMatrix * _rotation.asMatrix() * scaleMatrix;
}

void Transformable::setModelMatrix(const Matrix4f& matrix){
	_modelMatrix = matrix;
}

} /* namespace burn */
