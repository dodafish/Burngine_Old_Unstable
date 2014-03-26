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

#include <Burngine/Physics/World.h>

#include <Burngine/Physics/RigidBody.h>
#include <Burngine/System/Reporter.h>

#include <bullet/btBulletDynamicsCommon.h>

#include <Burngine/System/Message.h>

#include <iostream>

namespace burn {

void World::onMessageReceive(const Message& message) {
	if(message.getName() == mn::RIGIDBODY_DESTRUCTED){
		Uint64 recId = 0;
		if(message.getParameter<Uint64>(mp::COMPONENT_ID, &recId)){
			removeRigidBodyById(recId);
		}
	}
}

void World::removeRigidBodyById(const Uint64& id) {

	//Check if we already have added the rigidbody
	for(size_t i = 0; i < _rigidBodies.size(); ++i){
		if(_rigidBodies[i]->getId().get() == id){

			_world->removeRigidBody(_rigidBodies[i]->getBulletRigidBody().get());

			_rigidBodies.erase(_rigidBodies.begin() + i);
			break;
		}
	}

}

World::World(const Vector3f& gravity) {
	_messageReceiver.bindReceiveFunction(std::bind(&World::onMessageReceive, this, std::placeholders::_1));

	_broadphase = new btDbvtBroadphase();

	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
	_solver = new btSequentialImpulseConstraintSolver;
	_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
	_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

World::~World() {
	delete _world;
	delete _solver;
	delete _dispatcher;
	delete _collisionConfiguration;
	delete _broadphase;
}

bool World::addRigidBody(RigidBody& body) {

	//Check if we already have added the rigidbody
	for(size_t i = 0; i < _rigidBodies.size(); ++i){
		if(_rigidBodies[i] == &body){
			Reporter::report("Adding of rigid body denied. Rigid body is already added!", Reporter::ERROR);
			return false;
		}
	}

	_world->addRigidBody(body.getBulletRigidBody().get());
	_rigidBodies.push_back(&body);

	return true;
}

void World::removeRigidBody(const RigidBody& body) {

	removeRigidBodyById(body.getId().get());

}

void World::stepSimulation(const float& elapsed) {
	_world->stepSimulation(elapsed, 10, 1.f / 40.f);
}

} /* namespace burn */
