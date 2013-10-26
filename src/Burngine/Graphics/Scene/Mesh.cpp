/*
 * Mesh.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include <Burngine/Graphics/Scene/Mesh.h>

#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/Window/Window.h>

namespace burn {

Mesh::Mesh() :
_needUpdate(false) {

	_positionVbo.create();
	_colorVbo.create();
	_uvVbo.create();
	_normalVbo.create();

}

bool Mesh::update() {
	bool updated = false;
	if(_needUpdate){
		updated = data();
		_needUpdate = !updated;
	}
	return updated;
}

void Mesh::forceUpdate() {
	_needUpdate = true;
}

size_t Mesh::getVertexCount() const {
	return _vertices.size();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices, bool updateImmediatly) {
	_vertices = vertices;
	_needUpdate = true;
	if(updateImmediatly)
		update();
}

const VertexBufferObject& Mesh::getPositionVbo() const {
	return _positionVbo;
}

const VertexBufferObject& Mesh::getNormalVbo() const {
	return _normalVbo;
}

const VertexBufferObject& Mesh::getColorVbo() const {
	return _colorVbo;
}

const VertexBufferObject& Mesh::getUvVbo() const {
	return _uvVbo;
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

void Mesh::setMaterial(Material& material) {
	_material = material;
}

bool Mesh::data() {
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

		return true;

	}
	return false;
}

bool Mesh::isUpdated() const{
	return (!_needUpdate);
}

} /* namespace burn */
