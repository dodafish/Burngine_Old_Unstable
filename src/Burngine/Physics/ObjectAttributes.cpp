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

#include <Burngine/Physics/ObjectAttributes.h>

namespace burn {

void ObjectAttributes::setLinearVelocity(const Vector3f& linearVelocity) {
	_linearVelocity = linearVelocity;
}
void ObjectAttributes::setAngularVelocity(const Vector3f& angularVelocity) {
	_angularVelocity = angularVelocity;
}
void ObjectAttributes::setFriction(const float& friction) {
	_friction = friction;
}
void ObjectAttributes::setRestitution(const float& restitution) {
	_restitution = restitution;
}

const Vector3f& ObjectAttributes::getLinearVelocity() const {
	return _linearVelocity;
}
const Vector3f& ObjectAttributes::getAngularVelocity() const {
	return _angularVelocity;
}
const float& ObjectAttributes::getFriction() const {
	return _friction;
}
const float& ObjectAttributes::getRestitution() const {
	return _restitution;
}

} /* namespace burn */
