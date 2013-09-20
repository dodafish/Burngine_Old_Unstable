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
				_vertexPositionBuffer(0),
				_vertexColorBuffer(0),
				_vertexUvBuffer(0),
				_vertexNormalBuffer(0),
				_materialIndex(0) {

	if(Window::isContextCreated()){
		glGenBuffers(1, &_vertexPositionBuffer);
		glGenBuffers(1, &_vertexColorBuffer);
		glGenBuffers(1, &_vertexUvBuffer);
		glGenBuffers(1, &_vertexNormalBuffer);
	}

}

Mesh::~Mesh() {

	if(Window::isContextCreated()){
		glDeleteBuffers(1, &_vertexPositionBuffer);
		glDeleteBuffers(1, &_vertexColorBuffer);
		glDeleteBuffers(1, &_vertexUvBuffer);
		glDeleteBuffers(1, &_vertexNormalBuffer);
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

void Mesh::setDiffuseColor(const Vector3f& color) {
	for(size_t i = 0; i < _vertices.size(); ++i){
		_vertices[i].setColor(color);
	}
	_needUpdate = true;
	data();
}

const GLuint& Mesh::getPositionBuffer() const {
	return _vertexPositionBuffer;
}

const GLuint& Mesh::getNormalBuffer() const {
	return _vertexNormalBuffer;
}

const GLuint& Mesh::getColorBuffer() const {
	return _vertexColorBuffer;
}

const GLuint& Mesh::getUvBuffer() const {
	return _vertexUvBuffer;
}

void Mesh::setTexture(const Texture& tex) {
	_texture = tex;
}

const Texture& Mesh::getTexture() const {
	return _texture;
}

const Material& Mesh::getMaterial() const {
	return _material;
}

void Mesh::setMaterial(const Material& material) {
	_material = material;
}

void Mesh::data() {
	if(_needUpdate && Window::isContextCreated() && _vertices.size() != 0){

		if(_vertexPositionBuffer == 0){
			glGenBuffers(1, &_vertexPositionBuffer);
			glGenBuffers(1, &_vertexColorBuffer);
			glGenBuffers(1, &_vertexUvBuffer);
			glGenBuffers(1, &_vertexNormalBuffer);
		}

		std::vector<GLfloat> pos, col, uv, norm;
		for(size_t i = 0; i < _vertices.size(); ++i){
			pos.push_back(_vertices[i].getPosition().x);
			pos.push_back(_vertices[i].getPosition().y);
			pos.push_back(_vertices[i].getPosition().z);

			col.push_back(_vertices[i].getColor().r);
			col.push_back(_vertices[i].getColor().g);
			col.push_back(_vertices[i].getColor().b);

			uv.push_back(_vertices[i].getUv().x);
			uv.push_back(_vertices[i].getUv().y);

			norm.push_back(_vertices[i].getNormal().x);
			norm.push_back(_vertices[i].getNormal().y);
			norm.push_back(_vertices[i].getNormal().z);
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vertexPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pos.size(), &pos[0],
		GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * col.size(), &col[0],
		GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexUvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uv.size(), &uv[0],
		GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * norm.size(), &norm[0],
		GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind

		_needUpdate = false;
	}
}

void Mesh::setMaterialIndex(const unsigned int& index) {
	_materialIndex = index;
}

const unsigned int& Mesh::getMaterialIndex() const {
	return _materialIndex;
}

} /* namespace burn */
