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

#include <Burngine/System/Rotation.h>

namespace burn {

Rotation::Rotation(const Quaternion& quat) :
_quat(quat) {
	updateMatrix();
}

void Rotation::updateMatrix() {
	_mat = glm::toMat4(_quat);
}

const Quaternion& Rotation::asQuaternion() const {
	return _quat;
}

const Matrix4f& Rotation::asMatrix() const {
	return _mat;
}

void Rotation::setByEulerInRadians(const Vector3f& euler) {
	_quat = Quaternion(euler);
	updateMatrix();
}

void Rotation::setByEulerInDegrees(const Vector3f& euler) {
	_quat = Quaternion(glm::radians(euler));
	updateMatrix();
}

void Rotation::setByQuaternion(const Quaternion& quat) {
	_quat = quat;
	updateMatrix();
}

void Rotation::setByAxisAngleInRadians(	const Vector3f& axis,
										const float& angle) {
	_quat = glm::angleAxis(glm::degrees(angle), axis);
	updateMatrix();
}

void Rotation::setByAxisAngleInDegrees(	const Vector3f& axis,
										const float& angle) {
	_quat = glm::angleAxis(angle, axis);
	updateMatrix();
}

void Rotation::setByBulletQuaternion(const btQuaternion& bulletQuat) {

	burn::Quaternion q = burn::Quaternion(-bulletQuat.z(), bulletQuat.y(), -bulletQuat.x(), bulletQuat.w());
	burn::Rotation r;
	r.setByAxisAngleInDegrees(burn::Vector3f(0.f, 0.f, 1.f), 180.f);
	q = q * r.asQuaternion();

	_quat = q;
	updateMatrix();
}

btQuaternion Rotation::asBulletQuaternion() const {
	return btQuaternion(_quat.x, _quat.y, _quat.z, _quat.w);
}

} /* namespace burn */
