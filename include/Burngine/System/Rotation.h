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

#ifndef ROTATION_H_
#define ROTATION_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>
#include <LinearMath/btQuaternion.h>

namespace burn {

class BURNGINE_API Rotation {
public:
	Rotation(const Quaternion& quat = Quaternion());

	//Getters:
	const Quaternion& asQuaternion() const;
	const Matrix4f& asMatrix() const;

	btQuaternion asBulletQuaternion() const;

	//Setters:
	void setByEulerInRadians(const Vector3f& euler);
	void setByEulerInDegrees(const Vector3f& euler);

	void setByQuaternion(const Quaternion& quat);

	void setByAxisAngleInRadians(const Vector3f& axis, const float& angle);
	void setByAxisAngleInDegrees(const Vector3f& axis, const float& angle);

	void setByBulletQuaternion(const btQuaternion& bulletQuat);

private:
	Quaternion _quat;
	Matrix4f _mat;

	// Quat -> Mat
	void updateMatrix();
};

} /* namespace burn */
#endif /* ROTATION_H_ */
