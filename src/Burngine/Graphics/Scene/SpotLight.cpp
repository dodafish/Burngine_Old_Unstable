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

#include <Burngine/Graphics/Scene/SpotLight.h>

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Window/Window.h>

namespace burn {

SpotLight::SpotLight() :
_coneAngle(30.f) {
	_shadowMap.create(ShadowMap::HIGH);
}

void SpotLight::setConeAngle(const float& angle) {
	_coneAngle = angle;

	if(_coneAngle <= 0.f)
		_coneAngle = 1.f;
	else if(_coneAngle >= 90.f)
		_coneAngle = 89.f;

}

const float& SpotLight::getConeAngle() const {
	return _coneAngle;
}

void SpotLight::updateShadowMap(const std::vector<SceneNode*>& nodes) {

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
	Matrix4f projectionMatrix = glm::perspective<float>(90.f - _coneAngle, 1.f, 0.1f, 100.f);
	Matrix4f viewMatrix = glm::lookAt(_position, _position + lightDir, glm::vec3(0, 1, 0));

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
