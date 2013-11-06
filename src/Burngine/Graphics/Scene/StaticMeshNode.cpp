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

#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/Scene/Material.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Scene/SpotLight.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <iostream>

namespace burn {

const Matrix4f MVP_TO_BIAS(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0);

StaticMeshNode::StaticMeshNode() {
}

StaticMeshNode::~StaticMeshNode() {
}

void StaticMeshNode::setModel(const Model& model) {
	_model = model;
}

const Model& StaticMeshNode::getModel() const {
	return _model;
}

bool StaticMeshNode::loadFromFile(const std::string& file) {
	return _model.loadFromFile(file);
}

void StaticMeshNode::setFlag(const Material::Flag& flag, const bool& enabled) {
	_model.setFlag(flag, enabled);
}

void StaticMeshNode::update() {
	_model.update();
}

void StaticMeshNode::draw(const Camera& cam) {

	if(Window::isContextCreated()){

		_model.update();

		for(size_t i = 0; i < _model.getMeshCount(); ++i){

			if(_model.getMesh(i).getMaterial().getType() == Material::Type::SOLID_COLOR){

				//Get shader
				const Shader& shader = BurngineShaders::getShader(BurngineShaders::COLOR);

				//Set uniforms
				setMVPUniforms(shader, cam);

				//0 = Positions
				glEnableVertexAttribArray(0);
				_model.getMesh(i).getPositionVbo().bind();
				glVertexAttribPointer(0, // attribute 0
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

				//1 = Colors
				glEnableVertexAttribArray(1);
				_model.getMesh(i).getColorVbo().bind();
				glVertexAttribPointer(1, // attribute 1
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

				//Draw
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, _model.getMesh(i).getVertexCount(), shader);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

			}else if(_model.getMesh(i).getMaterial().getType() == Material::Type::TEXTURED){

				//Get shader
				const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURE);

				//Set uniforms
				setMVPUniforms(shader, cam);

				_model.getMesh(i).getTexture().bindAsSource();

				//0 = Positions
				glEnableVertexAttribArray(0);
				_model.getMesh(i).getPositionVbo().bind();
				glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

				//1 = UVs
				glEnableVertexAttribArray(1);
				_model.getMesh(i).getUvVbo().bind();
				glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

				//Draw
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, _model.getMesh(i).getVertexCount(), shader);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

			}
		}

	}

}

void StaticMeshNode::drawSingleColor(const Camera& cam, const Vector4f& color) {

	_model.update();

	for(size_t i = 0; i < _model.getMeshCount(); ++i){

		//Get shader
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::SINGLECOLOR);

		//Set uniforms
		setMVPUniforms(shader, cam);
		shader.setUniform("color", color);

		//0 = Positions
		glEnableVertexAttribArray(0);
		_model.getMesh(i).getPositionVbo().bind();
		glVertexAttribPointer(0, // attribute 0
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

		//Draw
		OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, _model.getMesh(i).getVertexCount(), shader);

		glDisableVertexAttribArray(0);

	}

}

void StaticMeshNode::drawLighting(LightingType type, const Camera& cam, const std::vector<Light*>& lights,
const Vector3f& ambient) {

	_model.update();

	Vector3f camPosition = cam.getPosition();

	for(size_t j = 0; j < lights.size(); ++j){
		for(size_t i = 0; i < _model.getMeshCount(); ++i){

			//Check if lighting is enabled
			if(!_model.getMesh(i).getMaterial().isFlagSet(Material::LIGHTING)){
				if(type == DIFFUSE){
					//Draw full white, so multiplication does not affect materialcolor
					drawSingleColor(cam, Vector4f(1.f));
					continue;
				}else{
					//Draw nothing. Specular will be added and adding 0 makes no difference
					continue;
				}
			}

			if(typeid(*(lights[j])) == typeid(Light)){ //It's a pointlight

				//Get shader
				const Shader& shader = BurngineShaders::getShader(BurngineShaders::POINTLIGHT);

				//Set uniforms
				setMVPUniforms(shader, cam);

				//Calculate matrices. Projection- and Viewmatrix used for Normalmatrix
				Matrix4f normalMatrix, view;
				view = glm::lookAt(cam.getPosition(), cam.getLookAt(), glm::vec3(0, 1, 0));
				normalMatrix = glm::transpose(glm::inverse(view * getModelMatrix()));

				shader.setUniform("normalMatrix", normalMatrix);
				shader.setUniform("cameraPosition", camPosition);
				shader.setUniform("lightPosition", lights[j]->getPosition());
				shader.setUniform("lightColor", lights[j]->getColor());
				shader.setUniform("ambientColor", ambient);
				shader.setUniform("specularColor", _model.getMesh(i).getMaterial().getSpecularColor());
				shader.setUniform("lightIntensity", lights[j]->getIntensity());
				shader.setUniform(
				"cubespaceMatrix",
				lights[j]->getBiasProjectionMatrix() * lights[j]->findViewMatrix(5));

				lights[j]->bindShadowCubeMap();

				if(type == DIFFUSE){
					shader.setUniform("lightingType", 1);
				}else{
					shader.setUniform("lightingType", 2);
				}

				//0 = Positions
				glEnableVertexAttribArray(0);
				_model.getMesh(i).getPositionVbo().bind();
				glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
				);

				//1 = Normals
				glEnableVertexAttribArray(1);
				_model.getMesh(i).getNormalVbo().bind();
				glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
				);

				//Draw
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, _model.getMesh(i).getVertexCount(), shader);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

			}else if(typeid(*(lights[j])) == typeid(SpotLight)){

				SpotLight* light = static_cast<SpotLight*>(lights[j]);

				//Get shader
				const Shader& shader = BurngineShaders::getShader(BurngineShaders::SPOTLIGHT);

				//Set uniforms
				setMVPUniforms(shader, cam);

				//Calculate matrices. Projection- and Viewmatrix used for Normalmatrix
				Matrix4f normalMatrix, view;
				view = glm::lookAt(cam.getPosition(), cam.getLookAt(), glm::vec3(0, 1, 0));
				normalMatrix = glm::transpose(glm::inverse(view * getModelMatrix()));

				//Calculate some values for the lightsource
				float lightConeCosine = std::cos(light->getConeAngle() / (180.f / 3.1415f));
				Vector4f lightDir = light->getDirection();

				shader.setUniform("normalMatrix", normalMatrix);
				shader.setUniform("cameraPosition", camPosition);
				shader.setUniform("lightPosition", light->getPosition());
				shader.setUniform("lightColor", light->getColor());
				shader.setUniform("ambientColor", ambient);
				shader.setUniform("specularColor", _model.getMesh(i).getMaterial().getSpecularColor());
				shader.setUniform("lightIntensity", light->getIntensity());
				shader.setUniform("cutoffAngle", light->getConeAngle());
				shader.setUniform("lightConeCosine", lightConeCosine);
				shader.setUniform("lightDirection", Vector3f(lightDir.x, lightDir.y, lightDir.z));
				shader.setUniform(
				"depthBiasMvp",
				MVP_TO_BIAS * light->getBiasProjectionMatrix() * light->getBiasViewMatrix() * getModelMatrix());

				light->bindShadowMap();

				if(type == DIFFUSE){
					shader.setUniform("lightingType", 1);
				}else{
					shader.setUniform("lightingType", 2);
				}

				//0 = Positions
				glEnableVertexAttribArray(0);
				_model.getMesh(i).getPositionVbo().bind();
				glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
				);

				//1 = Normals
				glEnableVertexAttribArray(1);
				_model.getMesh(i).getNormalVbo().bind();
				glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
				);

				//Draw
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, _model.getMesh(i).getVertexCount(), shader);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

			}else{ //Directional Light

				DirectionalLight* light = static_cast<DirectionalLight*>(lights[j]);

				//Get shader
				const Shader& shader = BurngineShaders::getShader(BurngineShaders::DIRECTIONAL_LIGHT);

				//Set uniforms
				setMVPUniforms(shader, cam);

				//Calculate matrices. Projection- and Viewmatrix used for Normalmatrix
				Matrix4f normalMatrix, view;
				view = glm::lookAt(cam.getPosition(), cam.getLookAt(), glm::vec3(0, 1, 0));
				normalMatrix = glm::transpose(glm::inverse(view * getModelMatrix()));

				//Calculate some values for the lightsource
				Vector4f lightDir = light->getDirection();

				shader.setUniform("normalMatrix", normalMatrix);
				shader.setUniform("cameraPosition", camPosition);
				shader.setUniform("lightColor", light->getColor());
				shader.setUniform("specularColor", _model.getMesh(i).getMaterial().getSpecularColor());
				shader.setUniform("lightIntensity", light->getIntensity());
				shader.setUniform("lightDirection", Vector3f(lightDir.x, lightDir.y, lightDir.z));
				shader.setUniform(
				"depthBiasMvp",
				MVP_TO_BIAS * light->getBiasProjectionMatrix() * light->getBiasViewMatrix() * getModelMatrix());

				light->bindShadowMap();

				if(type == DIFFUSE){
					shader.setUniform("lightingType", 1);
				}else{
					shader.setUniform("lightingType", 2);
				}

				//0 = Positions
				glEnableVertexAttribArray(0);
				_model.getMesh(i).getPositionVbo().bind();
				glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
				);

				//1 = Normals
				glEnableVertexAttribArray(1);
				_model.getMesh(i).getNormalVbo().bind();
				glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
				);

				//Draw
				OpenGlControl::draw(OpenGlControl::TRIANGLES, 0, _model.getMesh(i).getVertexCount(), shader);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

			}

		}
	}

}

} /* namespace burn */
