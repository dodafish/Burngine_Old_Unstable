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

void StaticMeshNode::setMesh(const Mesh& mesh) {
	_mesh = mesh;
}

const Mesh& StaticMeshNode::getMesh() const {
	return _mesh;
}

void StaticMeshNode::draw(Camera* cam) {

	if(Window::isContextCreated()){

		glm::mat4 MVP;
		if(cam != nullptr){
			Matrix4f Projection = glm::perspective(cam->getFov(), cam->getAspectRatio(), 0.1f, 100.0f);
			Matrix4f View = glm::lookAt(cam->getPosition(), cam->getLookAt(), glm::vec3(0, 1, 0));
			MVP = Projection * View * getModelMatrix();
		}else{
			Matrix4f Projection = Matrix4f(1.f);
			Matrix4f View = Matrix4f(1.f);
			MVP = Projection * View * getModelMatrix();
		}

		if(_material.getType() == Material::SOLID_COLOR){

			BurngineShaders::useShader(BurngineShaders::SOLID_COLOR);

			glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::SOLID_COLOR, MVP_UNIFORM), 1,
			GL_FALSE, &MVP[0][0]);

			//0 = Positions
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, _mesh.getPositionBuffer());
			glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
					);

			//1 = Colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, _mesh.getColorBuffer());
			glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
					);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, _mesh.getVertexCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

		}else if(_material.getType() == Material::TEXTURED){

			BurngineShaders::useShader(BurngineShaders::TEXTURED);

			glUniformMatrix4fv(BurngineShaders::getShaderUniformLocation(BurngineShaders::TEXTURED, MVP_UNIFORM), 1,
			GL_FALSE, &MVP[0][0]);

			glBindTexture(GL_TEXTURE_2D, _mesh.getTexture().getTextureBuffer());

			//0 = Positions
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, _mesh.getPositionBuffer());
			glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
					);

			//1 = UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, _mesh.getUvBuffer());
			glVertexAttribPointer(1, // attribute 0. No particular reason for 0, but must match the layout in the shader.
					2,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
					);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, _mesh.getVertexCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

		}

	}

}

} /* namespace burn */
