/*
 * Camera.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include "Camera.h"

namespace burn {

Camera::Camera() :
_aspectRatio(16.f / 9.f),
_fov(45.f) {

}

Camera::~Camera() {
	Scene* parents[_parents.size()];
	for(size_t i = 0; i != _parents.size(); ++i)
		parents[i] = _parents[i];
	size_t size = _parents.size();
	for(size_t i = 0; i != size; ++i)
		parents[i]->detachCamera(*this);
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
	if(_fov > 90.f)
		_fov = 90.f;
	else if(_fov < 0.f)
		_fov = 0.f;
	else
		_fov = fov;
}

const float& Camera::getFov() const {
	return _fov;
}

} /* namespace burn */
