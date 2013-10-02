/*
 * SceneNode.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "SceneNode.h"

#include "Camera.h"
#include "Light.h"

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

void SceneNode::setMVPUniforms(const BurngineShaders::Type& type, Camera* cam) {

	glm::mat4 viewMatrix, projectionMatrix;
	Vector3f cameraPosition; //(0,0,0)
	if(cam != nullptr){
		projectionMatrix = glm::perspective(cam->getFov(), cam->getAspectRatio(), 0.1f, 100.0f);
		viewMatrix = glm::lookAt(cam->getPosition(), cam->getLookAt(), glm::vec3(0, 1, 0));
		cameraPosition = cam->getPosition();
	}else{
		projectionMatrix = Matrix4f(1.f);
		viewMatrix = Matrix4f(1.f);
	}

	glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(type, MODEL_MATRIX), 1,
	GL_FALSE, &getModelMatrix()[0][0]);
	glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(type, VIEW_MATRIX), 1,
	GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(type, PROJECTION_MATRIX), 1,
	GL_FALSE, &projectionMatrix[0][0]);

}

} /* namespace burn */
