/*
 * StaticMeshNode.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "StaticMeshNode.h"
#include "Window.h"
#include "Shader.h"
#include "DepthCubeMap.h"
#include "Material.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

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

				BurngineShaders::useShader(BurngineShaders::SOLID_COLOR);
				setMVPUniforms(BurngineShaders::SOLID_COLOR, cam);

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

				// Draw the triangle !
				glDrawArrays(GL_TRIANGLES, 0, _model.getMesh(i).getVertexCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

			}else if(_model.getMesh(i).getMaterial().getType() == Material::Type::TEXTURED){

				BurngineShaders::useShader(BurngineShaders::TEXTURED);
				setMVPUniforms(BurngineShaders::TEXTURED, cam);

				glBindTexture(GL_TEXTURE_2D, _model.getMesh(i).getTexture().getTextureBuffer());

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

				// Draw the triangles !
				glDrawArrays(GL_TRIANGLES, 0, _model.getMesh(i).getVertexCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

			}
		}

	}

}

void StaticMeshNode::drawDepthColorless(const Camera& cam) {

	_model.update();

	for(size_t i = 0; i < _model.getMeshCount(); ++i){

		BurngineShaders::useShader(BurngineShaders::COLORLESS);
		setMVPUniforms(BurngineShaders::COLORLESS, cam);

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

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, _model.getMesh(i).getVertexCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

		glDisableVertexAttribArray(0);

	}

}

void StaticMeshNode::drawLighting(const Camera& cam, const std::vector<Light*>& lights, const Vector3f& ambient) {

	_model.update();

	Vector3f camPosition = cam.getPosition();

	for(size_t j = 0; j < lights.size(); ++j){
		for(size_t i = 0; i < _model.getMeshCount(); ++i){

			//---
			//Draw the actual lighting:
			BurngineShaders::useShader(BurngineShaders::LIGHTING);
			setMVPUniforms(BurngineShaders::LIGHTING, cam);

			Matrix4f normalMatrix, view, projection;
			projection = glm::perspective(cam.getFov(), cam.getAspectRatio(), 0.1f, 100.0f);
			view = glm::lookAt(cam.getPosition(), cam.getLookAt(), glm::vec3(0, 1, 0));
			normalMatrix = projection * view * glm::transpose(glm::inverse(getModelMatrix()));

			if(_model.getMesh(i).getMaterial().isFlagSet(Material::LIGHTING)){
				glUniform1i(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, LIGHT_ENABLED), 1);
			}else{
				glUniform1i(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, LIGHT_ENABLED), 0);
			}

			glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, NORMAL_MATRIX), 1,
			GL_FALSE, &normalMatrix[0][0]);

			glUniform3f(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, CAMERA_POSITION),
			camPosition.x, camPosition.y, camPosition.z);

			glUniform3f(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, LIGHT_POSITION),
			lights[j]->getPosition().x, lights[j]->getPosition().y, lights[j]->getPosition().z);

			glUniform3f(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, LIGHT_COLOR),
			lights[j]->getColor().r, lights[j]->getColor().g, lights[j]->getColor().b);

			glUniform3f(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, LIGHT_AMBIENT), ambient.r,
			ambient.g, ambient.b);

			glUniform3f(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, LIGHT_SPECULAR),
			_model.getMesh(i).getMaterial().getSpecularColor().r, _model.getMesh(i).getMaterial().getSpecularColor().g,
			_model.getMesh(i).getMaterial().getSpecularColor().b);

			glUniform1f(BurngineShaders::getShaderUniformLocation(BurngineShaders::LIGHTING, LIGHT_INTENSITY),
			lights[j]->getIntensity());

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

			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, _model.getMesh(i).getVertexCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

		}
	}

}

} /* namespace burn */
