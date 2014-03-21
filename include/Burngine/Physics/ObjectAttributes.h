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

#ifndef OBJECTATTRIBUTES_H_
#define OBJECTATTRIBUTES_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

namespace burn {

class BURNGINE_API ObjectAttributes {
public:
	ObjectAttributes(	const float& mass = 0.f,
	                 	const float& friction = 1.f,
						const float& restitution = 0.f,
						const Vector3f& linearVelocity = Vector3f(0.f),
						const Vector3f& angularVelocity = Vector3f(0.f));

	void setLinearVelocity(const Vector3f& linearVelocity);
	void setAngularVelocity(const Vector3f& angularVelocity);
	void setFriction(const float& friction);
	void setRestitution(const float& restitution);
	void setMass(const float& mass);

	const Vector3f& getLinearVelocity() const;
	const Vector3f& getAngularVelocity() const;
	const float& getFriction() const;
	const float& getRestitution() const;
	const float& getMass() const;
private:
	float _mass;
	Vector3f _linearVelocity, _angularVelocity;
	float _friction, _restitution;
};

} /* namespace burn */
#endif /* OBJECTATTRIBUTES_H_ */
