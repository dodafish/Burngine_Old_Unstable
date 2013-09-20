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

void SceneNode::setUniforms(const BurngineShaders::Type& type, std::shared_ptr<Camera> cam,
		const std::vector<std::shared_ptr<Light>>& lights) {

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

	glUniform3f(BurngineShaders::getShaderUniformLocation(type, CAMERA_POSITION), cameraPosition.x, cameraPosition.y,
			cameraPosition.z);

	glUniform1i(BurngineShaders::getShaderUniformLocation(type, LIGHT_COUNT), lights.size());

}

} /* namespace burn */
