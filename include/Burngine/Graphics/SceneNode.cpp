/*
 * SceneNode.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "SceneNode.h"

namespace burn {

SceneNode::SceneNode() {
}

SceneNode::~SceneNode() {
}

const Material& SceneNode::getMaterial() const {
	return _material;
}

void SceneNode::setMaterial(const Material& material) {
	_material = material;
}



} /* namespace burn */
