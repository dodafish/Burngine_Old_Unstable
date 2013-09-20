/*
 * StaticMeshNode.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "StaticMeshNode.h"
#include "Window.h"
#include "Shader.h"

namespace burn {

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

void StaticMeshNode::draw(std::shared_ptr<Camera> cam) {

	if(Window::isContextCreated()){

		_model.update();

		glm::mat4 viewMatrix, projectionMatrix;
		if(cam != nullptr){
			projectionMatrix = glm::perspective(cam->getFov(), cam->getAspectRatio(), 0.1f, 100.0f);
			viewMatrix = glm::lookAt(cam->getPosition(), cam->getLookAt(), glm::vec3(0, 1, 0));
		}else{
			projectionMatrix = Matrix4f(1.f);
			viewMatrix = Matrix4f(1.f);
		}

		for(size_t i = 0; i < _model.getMeshCount(); ++i){

			if(_model.getMesh(i).getMaterial().getType() == Material::Type::SOLID_COLOR){
				BurngineShaders::useShader(BurngineShaders::SOLID_COLOR);

				glUniformMatrix4fv(
						BurngineShaders::getShaderUniformLocation(BurngineShaders::SOLID_COLOR, MODEL_MATRIX), 1,
						GL_FALSE, &getModelMatrix()[0][0]);
				glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::SOLID_COLOR, VIEW_MATRIX),
						1,
						GL_FALSE, &viewMatrix[0][0]);
				glUniformMatrix4fv(
						BurngineShaders::getShaderUniformLocation(BurngineShaders::SOLID_COLOR, PROJECTION_MATRIX), 1,
						GL_FALSE, &projectionMatrix[0][0]);

				//0 = Positions
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, _model.getMesh(i).getPositionBuffer());
				glVertexAttribPointer(0, // attribute 0
						3,                  // size
						GL_FLOAT,           // type
						GL_FALSE,           // normalized?
						0,                  // stride
						(void*)0            // array buffer offset
						);

				//1 = Colors
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, _model.getMesh(i).getColorBuffer());
				glVertexAttribPointer(1, // attribute 1
						3,                  // size
						GL_FLOAT,           // type
						GL_FALSE,           // normalized?
						0,                  // stride
						(void*)0            // array buffer offset
						);

				//2 = Normals
				glEnableVertexAttribArray(2);
				glBindBuffer(GL_ARRAY_BUFFER, _model.getMesh(i).getNormalBuffer());
				glVertexAttribPointer(2, // attribute 2
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
				glDisableVertexAttribArray(2);

			}else if(_model.getMesh(i).getMaterial().getType() == Material::Type::TEXTURED){

				BurngineShaders::useShader(BurngineShaders::TEXTURED);

				glUniformMatrix4fv(
						BurngineShaders::getShaderUniformLocation(BurngineShaders::SOLID_COLOR, MODEL_MATRIX), 1,
						GL_FALSE, &getModelMatrix()[0][0]);
				glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::SOLID_COLOR, VIEW_MATRIX),
						1,
						GL_FALSE, &viewMatrix[0][0]);
				glUniformMatrix4fv(
						BurngineShaders::getShaderUniformLocation(BurngineShaders::SOLID_COLOR, PROJECTION_MATRIX), 1,
						GL_FALSE, &projectionMatrix[0][0]);

				glBindTexture(GL_TEXTURE_2D, _model.getMesh(i).getTexture().getTextureBuffer());

				//0 = Positions
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, _model.getMesh(i).getPositionBuffer());
				glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
						3,                  // size
						GL_FLOAT,           // type
						GL_FALSE,           // normalized?
						0,                  // stride
						(void*)0            // array buffer offset
						);

				//1 = UVs
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, _model.getMesh(i).getUvBuffer());
				glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
						2,                  // size
						GL_FLOAT,           // type
						GL_FALSE,           // normalized?
						0,                  // stride
						(void*)0            // array buffer offset
						);

				//2 = Normals
				glEnableVertexAttribArray(2);
				glBindBuffer(GL_ARRAY_BUFFER, _model.getMesh(i).getNormalBuffer());
				glVertexAttribPointer(2, // attribute 2
						3,                  // size
						GL_FLOAT,           // type
						GL_FALSE,           // normalized?
						0,                  // stride
						(void*)0            // array buffer offset
						);

				// Draw the triangles !
				glDrawArrays(GL_TRIANGLES, 0, _model.getMesh(i).getVertexCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(2);

			}
		}

	}

}

} /* namespace burn */
