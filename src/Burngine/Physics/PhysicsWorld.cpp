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

#include <Burngine/Physics/PhysicsWorld.h>

#include <Burngine/System/Log.h>

#include <bullet/btBulletDynamicsCommon.h>

#include <Burngine/System/Message.h>

#include <iostream>

namespace burn {

void PhysicsWorld::onMessageReceive(const Message& message) {
	if(message.getName() == mn::PHYSICALSCENENODE_DESTRUCTED){
		Uint64 recId = 0;
		if(message.getParameter<Uint64>(mp::COMPONENT_ID, &recId)){
			removePhysicalSceneNodeById(recId);
		}
	}
}

void PhysicsWorld::removePhysicalSceneNodeById(const Uint64& id) {

	//Check if we already have added the rigidbody
	for(size_t i = 0; i < _physicalNodes.size(); ++i){
		if(_physicalNodes[i].node->getId().get() == id){

			_world->removeRigidBody(_physicalNodes[i]._bulletRigidBody.get());

			_physicalNodes.erase(_physicalNodes.begin() + i);
			break;
		}
	}

}

PhysicsWorld::PhysicsWorld(const Vector3f& gravity) {
	_messageReceiver.bindReceiveFunction(std::bind(&PhysicsWorld::onMessageReceive, this, std::placeholders::_1));

	_broadphase = new btDbvtBroadphase();

	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
	_solver = new btSequentialImpulseConstraintSolver;
	_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
	_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

PhysicsWorld::~PhysicsWorld() {
	delete _world;
	delete _solver;
	delete _dispatcher;
	delete _collisionConfiguration;
	delete _broadphase;
}

bool PhysicsWorld::attachPhysicalSceneNode(StaticMeshNode& node) {
	for(size_t i = 0; i < _physicalNodes.size(); ++i){
		if(_physicalNodes[i].node == &node){
			return false;    //Already attached
		}
	}

	if(!node.getModel().isLoaded())
		return false;

	RigidSceneNode rsn;
	rsn.node = &node;

	rsn._bulletMotionState =
	std::shared_ptr<btMotionState>(new btDefaultMotionState(btTransform(node.getRotation().asBulletQuaternion(),
																		btVector3(	node.getPosition().x,
																					node.getPosition().y,
																					node.getPosition().z))));

	float mass = node.getAttributes().getMass();

	btVector3 fallInertia(0, 0, 0);
	node.getModel().getCollisionShape()->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(	mass,
															rsn._bulletMotionState.get(),
															node.getModel().getCollisionShape().get(),
															fallInertia);

	rsn._bulletRigidBody = std::shared_ptr<btRigidBody>(new btRigidBody(rigidBodyCI));

	_world->addRigidBody(rsn._bulletRigidBody.get());

	_physicalNodes.push_back(rsn);

	return true;
}

void PhysicsWorld::detachPhysicalSceneNode(const PhysicalSceneNode& node) {
	removePhysicalSceneNodeById(node.getId().get());
}

void PhysicsWorld::stepSimulation(	const float& elapsed,
							bool updateNodes) {

	//Upload transform and attributes to physics world (has effect when changed)
	for(size_t i = 0; i < _physicalNodes.size(); ++i){

		std::shared_ptr<btRigidBody> rigidBody = _physicalNodes[i]._bulletRigidBody;
		const ObjectAttributes& attr = _physicalNodes[i].node->getAttributes();

		rigidBody->activate(true);

		rigidBody->setFriction(attr.getFriction());
		rigidBody->setRestitution(attr.getRestitution());
		rigidBody->setLinearVelocity(btVector3(	attr.getLinearVelocity().x,
												attr.getLinearVelocity().y,
												attr.getLinearVelocity().z));
		rigidBody->setAngularVelocity(btVector3(attr.getAngularVelocity().x,
												attr.getAngularVelocity().y,
												attr.getAngularVelocity().z));

	}

	_world->stepSimulation(elapsed, 10, 1.f / 40.f);

	if(updateNodes){
		for(size_t i = 0; i < _physicalNodes.size(); ++i){

			Transformable t = *(_physicalNodes[i].node);
			ObjectAttributes attr = _physicalNodes[i].node->getAttributes();
			std::shared_ptr<btRigidBody> rigidBody = _physicalNodes[i]._bulletRigidBody;

			attr.setLinearVelocity(Vector3f(rigidBody->getLinearVelocity().x(),
											rigidBody->getLinearVelocity().y(),
											rigidBody->getLinearVelocity().z()));
			attr.setAngularVelocity(Vector3f(	rigidBody->getAngularVelocity().x(),
												rigidBody->getAngularVelocity().y(),
												rigidBody->getAngularVelocity().z()));
			attr.setFriction(rigidBody->getFriction());
			attr.setRestitution(rigidBody->getRestitution());

			btTransform trans;
			_physicalNodes[i]._bulletMotionState->getWorldTransform(trans);

			t.setPosition(Vector3f(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
			burn::Rotation rotation;
			rotation.setByBulletQuaternion(trans.getRotation());
			t.setRotation(rotation);

			_physicalNodes[i].node->setPosition(t.getPosition());
			_physicalNodes[i].node->setScale(t.getScale());
			_physicalNodes[i].node->setRotation(t.getRotation());
			_physicalNodes[i].node->setAttributes(attr);
		}
	}else{
		for(size_t i = 0; i < _physicalNodes.size(); ++i){
			std::shared_ptr<btRigidBody> rigidBody = _physicalNodes[i]._bulletRigidBody;
			rigidBody->activate(true);
		}
	}
}

} /* namespace burn */
