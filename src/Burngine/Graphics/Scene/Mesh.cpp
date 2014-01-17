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

namespace burn {

Mesh::Mesh() {

	_positionVbo.create();
	_colorVbo.create();
	_uvVbo.create();
	_normalVbo.create();

}

size_t Mesh::getVertexCount() const {
	return _vertices.size();
}

void Mesh::setVertices(const std::vector<Vertex>& vertices) {
	_vertices = vertices;
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

void Mesh::setMaterial(const Material& material) {
	_material = material;
	update();
}

bool Mesh::update() {
	if(_vertices.size() != 0){

		ensureContext();

		_positionVbo.reset();
		_colorVbo.reset();
		_normalVbo.reset();
		_uvVbo.reset();

		std::vector<GLfloat> pos, col, uv, norm;
		for(size_t i = 0; i < _vertices.size(); ++i){

			_positionVbo.addData(&(_vertices[i].getPosition()), sizeof(Vector3f));

			_colorVbo.addData(&(_material.getDiffuseColor()), sizeof(Vector3f));

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

} /* namespace burn */
