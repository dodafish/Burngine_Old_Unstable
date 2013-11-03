/*
 * Light.cpp
 *
 *  Created on: 20.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/DirectionalLight.h>

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Window/Window.h>

namespace burn {

DirectionalLight::DirectionalLight() {
	_shadowMap.create(ShadowMap::VERY_HIGH);
}

Vector4f DirectionalLight::getDirection() const {
	Matrix4f rotMat = glm::rotate(Matrix4f(1.f), _rotation.x, Vector3f(1.f, 0.f, 0.f));
	rotMat = glm::rotate(rotMat, _rotation.y, Vector3f(0.f, 1.f, 0.f));
	rotMat = glm::rotate(rotMat, _rotation.z, Vector3f(0.f, 0.f, 1.f));
	return (rotMat * Vector4f(1.f, 0.f, 0.f, 1.0f));
}

void DirectionalLight::bindShadowMap() const {
	_shadowMap.bindAsSource();
}

void DirectionalLight::updateShadowMap(const std::vector<SceneNode*>& nodes) {

	if(!Window::isContextCreated() || !_shadowMap.isCreated())
		return;

	//Clear old shadowMap and bind it for rendering
	_shadowMap.clear();
	_shadowMap.bindAsRendertarget();

	//Select shader
	const Shader& shader = BurngineShaders::getShader(BurngineShaders::DEPTH);

	//Calculate matrices
	Vector4f lightDirTemp = getDirection();
	Vector3f lightDir(lightDirTemp.x, lightDirTemp.y, lightDirTemp.z);
	Matrix4f projectionMatrix = glm::ortho<float>(-50.f, 50.f, -50.f, 50.f, -50.f, 50.f);
	Matrix4f viewMatrix = glm::lookAt(-lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//Set uniforms
	shader.setUniform("viewMatrix", viewMatrix);
	shader.setUniform("projectionMatrix", projectionMatrix);
	_biasViewMatrix = viewMatrix;
	_biasProjectionMatrix = projectionMatrix;

	//Scan through all nodes
	for(size_t i = 0; i < nodes.size(); ++i){

		if(!nodes[i]->isCastingShadows())
			continue;

		if(typeid(*(nodes[i])) == typeid(StaticMeshNode)){
			StaticMeshNode* node = static_cast<StaticMeshNode*>(nodes[i]);

			node->update();

			//Skip if updating failed or is incomplete
			if(!node->getModel().isUpdated())
				continue;

			Matrix4f modelMatrix = node->getModelMatrix();
			shader.setUniform("modelMatrix", modelMatrix);

			for(size_t j = 0; j < node->getModel().getMeshCount(); ++j){

				//0 = Positions
				glEnableVertexAttribArray(0);
				node->getModel().getMesh(j).getPositionVbo().bind();
				glVertexAttribPointer(0, // attribute 0
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

				//Draw
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, node->getModel().getMesh(j).getVertexCount(), shader);

				glDisableVertexAttribArray(0);

			}

		}

	}

}

} /* namespace burn */
