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

#ifndef WORLD_H_
#define WORLD_H_

#include <Burngine/Export.h>
#include <Burngine/System/NonCopyable.h>
#include <Burngine/System/Math.h>
#include <Burngine/System/MessageReceiver.h>

#include <vector>
#include <memory>

//Include as little bullet as possible here
#include <Burngine/extern/bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <Burngine/extern/bullet/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <Burngine/extern/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <Burngine/extern/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <Burngine/extern/bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

namespace burn {

class RigidBody;

class BURNGINE_API World : public NonCopyable {
public:
	World(const Vector3f& gravity = Vector3f(0.f, -9.98f, 0.f));
	~World();

	bool addRigidBody(RigidBody& body);
	void removeRigidBody(const RigidBody& body);

	void stepSimulation(const float& elapsed);

private:
	MessageReceiver _messageReceiver;
	void onMessageReceive(const Message& message);

	btDiscreteDynamicsWorld* _world;
	btBroadphaseInterface* _broadphase;
	btDefaultCollisionConfiguration* _collisionConfiguration;
	btCollisionDispatcher* _dispatcher;
	btSequentialImpulseConstraintSolver* _solver;

	std::vector<RigidBody*> _rigidBodies;
	void removeRigidBodyById(const Uint64& id);
};

} /* namespace burn */
#endif /* WORLD_H_ */