//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#include <Burngine/Graphics/Scene/Mesh.h>
#include <Burngine/Graphics/Scene/Model.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/OpenGL.h>

#include <iostream>
#include <map>

namespace burn {

Mesh::Mesh() :
_verticesCount(0) {

	_positionVbo.create();
	_colorVbo.create();
	_uvVbo.create();
	_normalVbo.create();
	_indexVbo.create();

}

Mesh::Mesh(const Mesh& other) :
_material(other._material),
_texture(other._texture),
_vertices(other._vertices),
_positionVbo(other._positionVbo),
_colorVbo(other._colorVbo),
_uvVbo(other._uvVbo),
_normalVbo(other._normalVbo),
_verticesCount(other._verticesCount) {
	std::cout << "Evil copy!\n";
}
Mesh& Mesh::operator==(const Mesh& other) {

	if(this == &other)
		return *this;

	_material = (other._material);
	_texture = (other._texture);
	_vertices = (other._vertices);
	_positionVbo = (other._positionVbo);
	_colorVbo = (other._colorVbo);
	_uvVbo = (other._uvVbo);
	_normalVbo = (other._normalVbo);

	std::cout << "Evil copy!\n";
	return *this;
}
Mesh::~Mesh() {

}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
	_vertices = vertices;
	update();
}

const std::vector<Vertex>& Mesh::getVertices() const {
	return _vertices;
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

const VertexBufferObject& Mesh::getTangentVbo() const {
	return _tangentVbo;
}

const VertexBufferObject& Mesh::getBitangentVbo() const {
	return _bitangentVbo;
}

const VertexBufferObject& Mesh::getIndexVbo() const {
	return _indexVbo;
}

void Mesh::setTexture(const std::shared_ptr<Texture>& tex) {
	_texture = tex;
}

void Mesh::setNormalMap(const std::shared_ptr<Texture>& tex) {
	_normalMap = tex;
}

const Texture* Mesh::getTexture() const {
	if(_texture.use_count() == 0)
		return nullptr;
	return _texture.get();
}

const Texture* Mesh::getNormalMap() const {
	if(_normalMap.use_count() == 0)
			return nullptr;
	return _normalMap.get();
}

const Material& Mesh::getMaterial() const {
	return _material;
}

void Mesh::setMaterial(const Material& material) {
	bool updateNeeded = false;
	if(_material.getDiffuseColor() != material.getDiffuseColor())
		updateNeeded = true;

	_material = material;

	if(updateNeeded)
		update();
}

// Returns true iif v1 can be considered equal to v2
bool is_near(	float v1,
				float v2) {
	return fabs(v1 - v2) < 0.01f;
}

bool getSimilarVertexIndex(	Vertex& vertex,
							std::map<Vertex, unsigned short> & VertexToOutIndex,
							unsigned short & result) {
	std::map<Vertex, unsigned short>::iterator it = VertexToOutIndex.find(vertex);
	if(it == VertexToOutIndex.end()){
		return false;
	}else{
		result = it->second;
		return true;
	}
}

void computeTangentBasis(std::vector<Vertex> & vertices) {

	for(size_t i = 0; i < vertices.size(); i += 3){

		// Shortcuts for vertices
		const glm::vec3 & v0 = vertices[i + 0].getPosition();
		const glm::vec3 & v1 = vertices[i + 1].getPosition();
		const glm::vec3 & v2 = vertices[i + 2].getPosition();

		// Shortcuts for UVs
		const glm::vec2 & uv0 = vertices[i + 0].getUv();
		const glm::vec2 & uv1 = vertices[i + 1].getUv();
		const glm::vec2 & uv2 = vertices[i + 2].getUv();

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		vertices[i + 0].setTangent(tangent);
		vertices[i + 1].setTangent(tangent);
		vertices[i + 2].setTangent(tangent);

		vertices[i + 0].setBitangent(bitangent);
		vertices[i + 1].setBitangent(bitangent);
		vertices[i + 2].setBitangent(bitangent);

	}

}

bool Mesh::update() {
	if(_vertices.size() != 0){

		computeTangentBasis(_vertices);

		//Run indexer:
		_indices.clear();
		_indexedVertices.clear();

		std::map<Vertex, unsigned short> VertexToOutIndex;

		//For each input vertex...
		for(size_t i = 0; i < _vertices.size(); ++i){

			//Try to find another vertex
			unsigned short index;
			bool found = getSimilarVertexIndex(_vertices[i], VertexToOutIndex, index);

			if(found){    // A similar vertex is already in the VBO, use it instead !
				_indices.push_back(index);

				_indexedVertices[index].setTangent(_indexedVertices[index].getTangent() + _vertices[i].getTangent());
				_indexedVertices[index].setBitangent(_indexedVertices[index].getBitangent()
				+ _vertices[i].getBitangent());

			}else{    // If not, it needs to be added in the output data.
				_indexedVertices.push_back(_vertices[i]);
				unsigned short newindex = (unsigned short)_indexedVertices.size() - 1;
				_indices.push_back(newindex);
				VertexToOutIndex[_vertices[i]] = newindex;
			}

		}

		ensureContext();

		_positionVbo.reset();
		_colorVbo.reset();
		_normalVbo.reset();
		_uvVbo.reset();
		_indexVbo.reset();
		_tangentVbo.reset();
		_bitangentVbo.reset();

		for(size_t i = 0; i < _indexedVertices.size(); ++i){

			_positionVbo.addData(&(_indexedVertices[i].getPosition()), sizeof(Vector3f));

			_colorVbo.addData(&(_material.getDiffuseColor()), sizeof(Vector3f));

			_uvVbo.addData(&(_indexedVertices[i].getUv()), sizeof(Vector2f));

			_normalVbo.addData(&(_indexedVertices[i].getNormal()), sizeof(Vector3f));

			_tangentVbo.addData(&(_indexedVertices[i].getTangent()), sizeof(Vector3f));
			_bitangentVbo.addData(&(_indexedVertices[i].getBitangent()), sizeof(Vector3f));

		}
		for(size_t i = 0; i < _indices.size(); ++i){
			_indexVbo.addData(&(_indices[i]), sizeof(unsigned short));
		}

		_positionVbo.uploadDataToGpu();
		_colorVbo.uploadDataToGpu();
		_normalVbo.uploadDataToGpu();
		_uvVbo.uploadDataToGpu();
		_tangentVbo.uploadDataToGpu();
		_bitangentVbo.uploadDataToGpu();

		_indexVbo.uploadDataToGpu(GL_ELEMENT_ARRAY_BUFFER);

		_verticesCount = _indices.size();

		return true;

	}
	return false;
}

} /* namespace burn */
