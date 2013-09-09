/*
 * SceneNode.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "SceneNode.h"

namespace burn {

SceneNode::SceneNode() {
	// TODO Auto-generated constructor stub

}

SceneNode::~SceneNode() {
	// TODO Auto-generated destructor stub
}

void SceneNode::setPosition(const Vector3f& position) {
	_position = position;
}

const Vector3f& SceneNode::getPosition() const {
	return _position;
}

void SceneNode::setRotation(const Vector3f& rotation) {
	_rotation = rotation;
}

const Vector3f& SceneNode::getRotation() const {
	return _rotation;
}

void SceneNode::setScale(const Vector3f& scale) {
	_scale = scale;
}

const Vector3f& SceneNode::getScale() const {
	return _scale;
}

} /* namespace burn */
