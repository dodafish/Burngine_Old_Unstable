/*
 * StaticMeshNode.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/StaticMeshNode.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/Scene/Material.h>
#include <Burngine/Graphics/Scene/Camera.h>
#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Scene/Light.h>
#include <Burngine/Graphics/General/OpenGlControl.h>

#include <iostream>

namespace burn {

StaticMeshNode::StaticMeshNode() {
}

StaticMeshNode::~StaticMeshNode() {
}

void StaticMeshNode::setModel(const Model& model) {
	_model = model;
}

Model& StaticMeshNode::getModel() {
	return _model;
}

bool StaticMeshNode::loadFromFile(const std::string& file) {
	return _model.loadFromFile(file);
}

void StaticMeshNode::draw(const Camera& cam) {

	if(Window::isContextCreated()){

		_model.update();

		for(size_t i = 0; i < _model.getMeshCount(); ++i){

			if(_model.getMesh(i).getMaterial().getType() == Material::Type::SOLID_COLOR){

				//Get shader
				const Shader& shader = BurngineShaders::getShader(BurngineShaders::SOLID_COLOR);

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
				const Shader& shader = BurngineShaders::getShader(BurngineShaders::TEXTURED);

				//Set uniforms
				setMVPUniforms(shader, cam);

				_model.getMesh(i).getTexture().bind();

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

void StaticMeshNode::drawDepthColorless(const Camera& cam) {

	_model.update();

	for(size_t i = 0; i < _model.getMeshCount(); ++i){

		//Get shader
		const Shader& shader = BurngineShaders::getShader(BurngineShaders::COLORLESS);

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

			//Get shader
			const Shader& shader = BurngineShaders::getShader(BurngineShaders::LIGHTING);

			//Set uniforms
			setMVPUniforms(shader, cam);

			//Calculate matrices. Projection- and Viewmatrix used for Normalmatrix
			Matrix4f normalMatrix, view, projection;
			projection = glm::perspective(cam.getFov(), cam.getAspectRatio(), 0.1f, 10000.0f);
			view = glm::lookAt(cam.getPosition(), cam.getLookAt(), glm::vec3(0, 1, 0));
			normalMatrix = projection * view * glm::transpose(glm::inverse(getModelMatrix()));

			if(_model.getMesh(i).getMaterial().isFlagSet(Material::LIGHTING)){
				shader.setUniform(LIGHT_ENABLED, 1);
			}else{
				shader.setUniform(LIGHT_ENABLED, 0);
			}

			shader.setUniform(NORMAL_MATRIX, normalMatrix);
			shader.setUniform(CAMERA_POSITION, camPosition);
			shader.setUniform(LIGHT_POSITION, lights[j]->getPosition());
			shader.setUniform(LIGHT_COLOR, lights[j]->getColor());
			shader.setUniform(LIGHT_AMBIENT, ambient);
			shader.setUniform(LIGHT_SPECULAR, _model.getMesh(i).getMaterial().getSpecularColor());
			shader.setUniform(LIGHT_INTENSITY, lights[j]->getIntensity());

			if(type == DIFFUSE){
				shader.setUniform(LIGHT_TYPE, 1);
			}else{
				shader.setUniform(LIGHT_TYPE, 2);
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

} /* namespace burn */
