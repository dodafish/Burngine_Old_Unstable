/*
 * StaticMeshNode.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "StaticMeshNode.h"
#include "Window.h"

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
