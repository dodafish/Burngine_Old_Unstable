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

#include <Burngine/Physics/RigidBody.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/extern/glm/gtc/type_ptr.hpp>

#include <Burngine/Graphics/Scene/Model.h>
#include <Burngine/Graphics/Scene/Mesh.h>

#include <bullet/btBulletDynamicsCommon.h>

#include <Burngine/System/Message.h>

namespace burn {

RigidBody::~RigidBody() {
	Message msg(mn::RIGIDBODY_DESTRUCTED);
	msg.addParameter<Uint64>(mp::COMPONENT_ID, _id.get());
	msg.send();
}

void RigidBody::forceSimulation() {

	if(_rigidBody.use_count() == 0)
		return;
	_rigidBody->activate(true);

}

void RigidBody::setCollisionShape(const std::shared_ptr<btCollisionShape>& shape) {
	_collisionShape = shape;
}

bool RigidBody::create(const float& mass) {

	if(_collisionShape.use_count() == 0){
		Reporter::report("Unable to create rigid body. No collision shape available!", Reporter::ERROR);
		return false;
	}

	_motionState.reset(new btDefaultMotionState(btTransform(_transform.getRotation().asBulletQuaternion(),
															btVector3(	_transform.getPosition().x,
																		_transform.getPosition().y,
																		_transform.getPosition().z))));

	btVector3 fallInertia(0, 0, 0);
	_collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, _motionState.get(), _collisionShape.get(), fallInertia);

	_rigidBody.reset(new btRigidBody(rigidBodyCI));

	return true;
}

void RigidBody::setTransform(const Transformable& transformable) {
	_transform = transformable;

	if(_rigidBody.use_count() == 0 || _motionState.use_count() == 0)
		return;

	{
		btTransform comTrans = _rigidBody->getCenterOfMassTransform();
		comTrans.setOrigin(btVector3(	_transform.getPosition().x,
										_transform.getPosition().y,
										_transform.getPosition().z));
		_rigidBody->setCenterOfMassTransform(comTrans);

	}
	//For translation, rotation and scale we use a matrix
	//btTransform trans;
	//trans.setFromOpenGLMatrix(glm::value_ptr(_transform.getModelMatrix()));
	//_rigidBody->setCenterOfMassTransform(trans);
	//_motionState->setWorldTransform(trans);

}
const Transformable& RigidBody::getTransform() const {
	return _transform;
}

void RigidBody::setAttributes(const ObjectAttributes& oa) {
	_attributes = oa;

	if(_rigidBody.use_count() == 0 || _motionState.use_count() == 0)
		return;

	_rigidBody->setFriction(_attributes.getFriction());
	_rigidBody->setRestitution(_attributes.getRestitution());
}

const ObjectAttributes& RigidBody::getAttributes() const {
	return _attributes;
}

void RigidBody::update() {

	if(_rigidBody.use_count() == 0 || _motionState.use_count() == 0)
		return;

	_attributes.setLinearVelocity(Vector3f(	_rigidBody->getLinearVelocity().x(),
											_rigidBody->getLinearVelocity().y(),
											_rigidBody->getLinearVelocity().z()));
	_attributes.setAngularVelocity(Vector3f(_rigidBody->getAngularVelocity().x(),
											_rigidBody->getAngularVelocity().y(),
											_rigidBody->getAngularVelocity().z()));
	_attributes.setFriction(_rigidBody->getFriction());
	_attributes.setRestitution(_rigidBody->getRestitution());

	btTransform trans;
	_motionState->getWorldTransform(trans);

	_transform.setPosition(Vector3f(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	burn::Rotation rotation;
	rotation.setByBulletQuaternion(trans.getRotation());
	_transform.setRotation(rotation);
}

const std::shared_ptr<btRigidBody>& RigidBody::getBulletRigidBody() const {
	return _rigidBody;
}

} /* namespace burn */
