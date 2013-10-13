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
_needUpdate(false) {

	_positionVbo.create();
	_colorVbo.create();
	_uvVbo.create();
	_normalVbo.create();

}

Mesh::~Mesh() {

	_positionVbo.cleanup();
	_colorVbo.cleanup();
	_uvVbo.cleanup();
	_normalVbo.cleanup();

}

void Mesh::update() {
	if(_needUpdate){
		data();
		_needUpdate = false;
	}
}

void Mesh::forceUpdate() {
	_needUpdate = true;
}

size_t Mesh::getVertexCount() const {
	return _vertices.size();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
	_vertices = vertices;
	_needUpdate = true;
}

VertexBufferObject& Mesh::getPositionVbo() {
	return _positionVbo;
}

VertexBufferObject& Mesh::getNormalVbo() {
	return _normalVbo;
}

VertexBufferObject& Mesh::getColorVbo() {
	return _colorVbo;
}

VertexBufferObject& Mesh::getUvVbo() {
	return _uvVbo;
}

void Mesh::setTexture(const Texture& tex) {
	_texture = tex;
}

const Texture& Mesh::getTexture() const {
	return _texture;
}

Material& Mesh::getMaterial() {
	return _material;
}

void Mesh::setMaterial(Material& material) {
	_material = material;
}

void Mesh::data() {
	if(Window::isContextCreated() && _vertices.size() != 0){

		_positionVbo.reset();
		_colorVbo.reset();
		_normalVbo.reset();
		_uvVbo.reset();

		std::vector<GLfloat> pos, col, uv, norm;
		for(size_t i = 0; i < _vertices.size(); ++i){
			_positionVbo.addData(&(_vertices[i].getPosition()), sizeof(Vector3f));

			if(!_material.isUsingDiffuseColor()){

				_colorVbo.addData(&(_vertices[i].getColor()), sizeof(Vector3f));
			}else{

				_colorVbo.addData(&(_material.getDiffuseColor()), sizeof(Vector3f));
			}

			_uvVbo.addData(&(_vertices[i].getUv()), sizeof(Vector2f));

			_normalVbo.addData(&(_vertices[i].getNormal()), sizeof(Vector3f));
		}

		_positionVbo.uploadDataToGpu();
		_colorVbo.uploadDataToGpu();
		_normalVbo.uploadDataToGpu();
		_uvVbo.uploadDataToGpu();

	}
}

} /* namespace burn */
