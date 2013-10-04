/*
 * Camera.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include "Camera.h"
#include "Scene.h"

namespace burn {

Camera::Camera() :
_aspectRatio(16.f / 9.f),
_fov(45.f),
_parent(nullptr) {

}

Camera::~Camera() {
	if(_parent != nullptr){
		_parent->setCamera(_parent->getDefaultCamera());
	}
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
