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

void StaticMeshNode::draw() {

	if(Window::isContextCreated()){

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View       = glm::lookAt(
		    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
		    glm::vec3(0,0,0), // and looks at the origin
		    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP        = Projection * View * Model;

		if(_material.getType() == Material::SOLID_COLOR){

			BurngineShaders::useShader(BurngineShaders::SOLID_COLOR);

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

		}

	}

}

} /* namespace burn */
