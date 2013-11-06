//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

#include <Burngine/Graphics/Scene/SceneNode.h>

#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Light.h>

namespace burn {

void SceneNode::removeAllParents() {
	Scene* parents[_parents.size()];
	for(size_t i = 0; i != _parents.size(); ++i)
		parents[i] = _parents[i];
	size_t size = _parents.size();
	for(size_t i = 0; i != size; ++i)
		parents[i]->detachSceneNode(*this);
}

SceneNode::SceneNode() :
_isCastingShadows(true) {
}

SceneNode::~SceneNode() {
	removeAllParents();
}

SceneNode::SceneNode(const SceneNode& other) :
_parents(other._parents),
_isCastingShadows(other._isCastingShadows) {
	for(size_t i = 0; i < _parents.size(); ++i){
		_parents[i]->attachSceneNode(*this);
	}
}

SceneNode& SceneNode::operator=(const SceneNode& other) {
	removeAllParents();

	_isCastingShadows = other._isCastingShadows;

	_parents = other._parents;
	for(size_t i = 0; i < _parents.size(); ++i){
		_parents[i]->attachSceneNode(*this);
	}
	return *this;
}

void SceneNode::addParentScene(Scene* scene) {
	for(size_t i = 0; i < _parents.size(); ++i){
		if(_parents[i] == scene)
			return; //Already added as parent
	}
	_parents.push_back(scene); //Add to parents
}

void SceneNode::removeParentScene(Scene* scene) {
	for(size_t i = 0; i < _parents.size(); ++i){
		if(_parents[i] == scene){
			_parents.erase(_parents.begin() + i);
			return; //addParentScene() ensures, that a scene is added only once. So return
		}
	}
}

void SceneNode::setMVPUniforms(const Shader& shader, const Camera& cam) {

	//Calculate matrices
	glm::mat4 projectionMatrix = glm::perspective(cam.getFov(), cam.getAspectRatio(), 0.1f, 10000.0f);
	glm::mat4 viewMatrix = glm::lookAt(cam.getPosition(), cam.getLookAt(), glm::vec3(0, 1, 0));

	//Set uniforms
	shader.setUniform("modelMatrix", getModelMatrix());
	shader.setUniform("viewMatrix", viewMatrix);
	shader.setUniform("projectionMatrix", projectionMatrix);

}

bool SceneNode::isCastingShadows() const {
	return _isCastingShadows;
}

void SceneNode::setCastingShadows(bool enabled) {
	_isCastingShadows = enabled;
}

} /* namespace burn */
