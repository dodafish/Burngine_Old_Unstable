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

namespace burn {

bool RigidBody::create(const float& mass) {

	if(_collisionShape.use_count() == 0){
		Reporter::report("Unable to create rigid body. No collision shape created!", Reporter::ERROR);
		return false;
	}

	_motionState.reset(new btDefaultMotionState(btTransform(_attributes.getRotation().asBulletQuaternion(),
															btVector3(	_attributes.getPosition().x,
																		_attributes.getPosition().y,
																		_attributes.getPosition().z))));

	btVector3 fallInertia(0, 0, 0);
	_collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, _motionState.get(), _collisionShape.get(), fallInertia);

	_rigidBody.reset(new btRigidBody(rigidBodyCI));

	return true;
}

bool RigidBody::createCollisionShape(	const Model& model,
										const SHAPE_PRECISION& precision) {

	if(!model.isLoaded()){
		Reporter::report("Unable to create collision shape. Model is not loaded!", Reporter::ERROR);
		return false;
	}

	if(precision == IDENTICAL){

		const std::vector<Mesh>& meshes = model.getMeshes();

		if(meshes.size() == 0){
			Reporter::report("Unable to create collision shape. Model has no meshes!", Reporter::ERROR);
			return false;
		}

		btConvexHullShape* convexHull = new btConvexHullShape;

		//Cycle through the meshes
		for(size_t i = 0; 1 < meshes.size(); ++i){
			const std::vector<Vertex>& vertices = meshes[i].getVertices();
			//Cycle through the vertices
			for(size_t j = 0; j != vertices.size(); ++j){
				convexHull->addPoint(btVector3(	vertices[j].getPosition().x,
												vertices[j].getPosition().y,
												vertices[j].getPosition().z));
			}
		}

		_collisionShape.reset(convexHull);
	}

	return true;
}

void RigidBody::setAttributes(const ObjectAttributes& oa) {
	_attributes = oa;

	if(_rigidBody.use_count() == 0 || _motionState.use_count() == 0)
		return;

	//For translation, rotation and scale we use a matrix
	btTransform trans;
	trans.setFromOpenGLMatrix(glm::value_ptr(_attributes.getModelMatrix()));
	//_rigidBody->setCenterOfMassTransform(trans);
	_motionState->setWorldTransform(trans);

	//Other
	_rigidBody->setFriction(_attributes.getFriction());
	_rigidBody->setRestitution(_attributes.getRestitution());
}

const ObjectAttributes& RigidBody::getAttributes() const {
	return _attributes;
}

void RigidBody::updateAttributes() {

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

	_attributes.setPosition(Vector3f(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	burn::Rotation rotation;
	rotation.setByBulletQuaternion(trans.getRotation());
	_attributes.setRotation(rotation);
}

} /* namespace burn */
