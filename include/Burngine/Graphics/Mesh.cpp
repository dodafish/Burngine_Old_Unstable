/*
 * Mesh.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Mesh.h"

#include "OpenGL.h"
#include "Window.h"

namespace burn {

Mesh::Mesh() :
				_needUpdate(false),
				_vertexPositionBuffer(0) {

	if(Window::isContextCreated()){
		glGenBuffers(1, &_vertexPositionBuffer);
	}

}

Mesh::~Mesh() {

	if(Window::isContextCreated()){
		glDeleteBuffers(1, &_vertexPositionBuffer);
	}

}

size_t Mesh::getVertexCount() const {
	return _vertices.size();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
	_vertices = vertices;
	_needUpdate = true;

	data();
}

const GLuint& Mesh::getPositionBuffer() {
	if(_needUpdate)
		data();
	return _vertexPositionBuffer;
}

void Mesh::data() {
	if(Window::isContextCreated() && _vertices.size() != 0){

		if(_vertexPositionBuffer == 0){
			glGenBuffers(1, &_vertexPositionBuffer);
		}

		std::vector<GLfloat> pos;
		for(size_t i = 0; i < _vertices.size(); ++i){
			pos.push_back(_vertices[i].getPosition().x);
			pos.push_back(_vertices[i].getPosition().y);
			pos.push_back(_vertices[i].getPosition().z);
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vertexPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pos.size(), &pos[0],
		GL_STATIC_DRAW);

		_needUpdate = false;
	}
}

} /* namespace burn */
