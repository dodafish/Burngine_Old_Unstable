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
_headUp(Vector3f(0.f, 1.f, 0.f)),
_fov(35.f),
_far(1000.f),
_near(0.1f),
_type(PERSPECTIVE) {

}

Camera::Camera(const Camera& other) :
_aspectRatio(other._aspectRatio),
_headUp(other._headUp),
_fov(other._fov),
_far(other._far),
_near(other._near),
_type(other._type) {

}

Camera& Camera::operator=(const Camera& other) {

	if(this == &other)
		return *this;

	_aspectRatio = other._aspectRatio;
	_headUp = other._headUp;
	_fov = other._fov;
	_far = other._far;
	_near = other._near;
	_type = other._type;

	return *this;
}

void Camera::setAspectRatio(const float& aspectRatio) {
	_aspectRatio = aspectRatio;
}

const float& Camera::getAspectRatio() const {
	return _aspectRatio;
}

void Camera::lookAt(const Vector3f& point) {
	_lookAt = point;
}

const Vector3f& Camera::getLookAt() const {
	return _lookAt;
}

void Camera::setFov(const float& fov) {
	if(_fov < 1.f)
		_fov = 1.f;
	else
		_fov = fov;
}

const float& Camera::getFov() const {
	return _fov;
}

Matrix4f Camera::getProjectionMatrix() const {

	if(_type == ORTHOGONAL)
		return (glm::ortho(	(_fov * _aspectRatio) / -2.f,
							(_fov * _aspectRatio) / 2.f,
							_fov / -2.f,
							_fov / 2.f,
							_near,
							_far));

	return (glm::perspective<float>(_fov, _aspectRatio, _near, _far));
}

Matrix4f Camera::getViewMatrix() const {
	return (glm::lookAt(_position, _lookAt, _headUp));
}

void Camera::setFar(const float& far) {
	_far = far;
}

void Camera::setNear(const float& near){
	_near = near;
}

void Camera::setType(const Type& type) {
	_type = type;
}

const Camera::Type& Camera::getType() const {
	return _type;
}

void Camera::setHeadUp(const Vector3f& headUp){
	_headUp = headUp;
}

} /* namespace burn */
