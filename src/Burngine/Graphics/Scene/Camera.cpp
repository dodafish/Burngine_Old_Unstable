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

#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Scene.h>

namespace burn {

#define NEAR 0.1f

Camera::Camera() :
_aspectRatio(16.f / 9.f),
_fov(35.f),
_far(1000.f) {

}

Camera::~Camera() {

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
	return (glm::perspective<float>(_fov, _aspectRatio, NEAR, _far));
}

Matrix4f Camera::getViewMatrix() const {
	return (glm::lookAt(_position, _lookAt, Vector3f(0.f, 1.f, 0.f)));
}

void Camera::setFar(const float& far) {
	_far = far;
}

} /* namespace burn */
