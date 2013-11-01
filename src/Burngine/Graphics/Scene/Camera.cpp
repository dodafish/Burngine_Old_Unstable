/*
 * Camera.cpp
 *
 *  Created on: 13.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Scene.h>

namespace burn {

Camera::Camera() :
_aspectRatio(16.f / 9.f),
_fov(35.f){

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

} /* namespace burn */
