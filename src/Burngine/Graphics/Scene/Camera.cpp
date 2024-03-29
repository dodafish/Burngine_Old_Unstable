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

#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Scene.h>

namespace burn {

Camera::Camera() :
_aspectRatio(1.f),
_fov(35.f),
_far(1000.f),
_near(0.1f),
_type(PERSPECTIVE),
_headUp(0.f, 1.f, 0.f) {
	_isChanged = true;
}

Camera::Camera(const Camera& other) :
_aspectRatio(other._aspectRatio),
_fov(other._fov),
_far(other._far),
_near(other._near),
_type(other._type),
_headUp(other._headUp) {

}

Camera& Camera::operator=(const Camera& other) {

	if(this == &other)
		return *this;

	_aspectRatio = other._aspectRatio;
	_fov = other._fov;
	_far = other._far;
	_near = other._near;
	_type = other._type;
	_headUp = other._headUp;

	return *this;
}

void Camera::setAspectRatio(const float& aspectRatio) {
	_isChanged = true;
	_aspectRatio = aspectRatio;
}

const float& Camera::getAspectRatio() const {
	return _aspectRatio;
}

void Camera::setFov(const float& fov) {
	if(_fov < 1.f)
		_fov = 1.f;
	else
		_fov = fov;
	_isChanged = true;
}

const float& Camera::getFov() const {
	return _fov;
}

void Camera::setProjectionMatrixOffset(const Matrix4f& offset){
	_projectionMatrixOffset = offset;
}

Matrix4f Camera::getProjectionMatrix() const {

	if(_isChanged){
		if(_type == ORTHOGONAL)
			_projectionMatrix = (glm::ortho((_fov * _aspectRatio) / -2.f,
											(_fov * _aspectRatio) / 2.f,
											_fov / -2.f,
											_fov / 2.f,
											_near,
											_far));
		else
			_projectionMatrix = (glm::perspective<float>(_fov, _aspectRatio > 0.f ? _aspectRatio :
																					1.f,
															_near, _far));

	}

	return _projectionMatrixOffset * _projectionMatrix;
}

Matrix4f Camera::getViewMatrix() const {
	if(_isChanged){
		Vector4f dir = _rotation.asMatrix() * Vector4f(0.f, 0.f, -1.f, 1.f);
		_viewMatrix = glm::lookAt(_position, _position + Vector3f(dir), _headUp);
	}
	return _viewMatrix;
}

void Camera::setFar(const float& far) {
	_isChanged = true;
	_far = far;
}

void Camera::setNear(const float& near) {
	_isChanged = true;
	_near = near;
}

void Camera::setType(const Type& type) {
	_isChanged = true;
	_type = type;
}

const Camera::Type& Camera::getType() const {
	return _type;
}

void Camera::setHeadUp(const Vector3f& headup) {
	_isChanged = true;
	_headUp = headup;
}

const Vector3f& Camera::getHeadUp() const {
	return _headUp;
}

void Camera::setViewMatrix(const Matrix4f& matrix) {
	//User has set the viewmatrix manually. so avoid automated resetting
	_isChanged = false;
	_viewMatrix = matrix;
}

void Camera::setProjectionMatrix(const Matrix4f& matrix) {
	//User has set the viewmatrix manually. so avoid automated resetting
	_isChanged = false;
	_projectionMatrix = matrix;
}

} /* namespace burn */
