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

#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_

#include <Burngine/Export.h>
#include <Burngine/Physics/ObjectAttributes.h>
#include <Burngine/Graphics/Scene/Transformable.h>

#include <Burngine/Physics/World.h>

//Include as little bullet as possible here
#include <Burngine/extern/bullet/BulletDynamics/Dynamics/btRigidBody.h>

//RigidBody will get copied, so we need smart pointers as we dont want
//to _really_ copy it.
#include <memory>

namespace burn {

class Model;

class BURNGINE_API RigidBody {
public:

	enum SHAPE_PRECISION {
		IDENTICAL
	};

	bool create(const float& mass);
	bool createCollisionShape(	const Model& model,
								const SHAPE_PRECISION& precision = IDENTICAL);

	void setTransform(const Transformable& transformable);
	const Transformable& getTransform() const;

	void setAttributes(const ObjectAttributes& oa);
	const ObjectAttributes& getAttributes() const;

	void update();

	void forceSimulation();

private:
	friend bool World::addRigidBody(const RigidBody&);
	friend bool World::removeRigidBody(const RigidBody&);

	std::shared_ptr<btRigidBody> _rigidBody;
	std::shared_ptr<btCollisionShape> _collisionShape;
	std::shared_ptr<btMotionState> _motionState;

	ObjectAttributes _attributes;
	Transformable _transform;
};

} /* namespace burn */
#endif /* RIGIDBODY_H_ */
