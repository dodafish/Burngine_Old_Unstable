/*
 * SceneNode.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/SceneNode.h>

#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Light.h>

namespace burn {

SceneNode::SceneNode() {
}

SceneNode::~SceneNode() {
	Scene* parents[_parents.size()];
	for(size_t i = 0; i != _parents.size(); ++i)
		parents[i] = _parents[i];
	size_t size = _parents.size();
	for(size_t i = 0; i != size; ++i)
		parents[i]->detachSceneNode(*this);
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
	glm::mat4 projectionMatrix = glm::perspective(cam.getFov(), cam.getAspectRatio(), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(cam.getPosition(), cam.getLookAt(), glm::vec3(0, 1, 0));

	//Set uniforms
	shader.setUniform(MODEL_MATRIX, getModelMatrix());
	shader.setUniform(VIEW_MATRIX, viewMatrix);
	shader.setUniform(PROJECTION_MATRIX, projectionMatrix);

}

} /* namespace burn */
