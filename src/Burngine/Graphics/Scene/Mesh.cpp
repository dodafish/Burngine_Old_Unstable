//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2013 Dominik David (frischer-hering@gmx.de)
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

Mesh::Mesh(const Model& model) :
_model(model),
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

void Mesh::setVertices(	const std::vector<Vertex>& vertices,
						bool updateImmediatly) {
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

void Mesh::setMaterial(const Material& material) {
	_material = material;
}

bool Mesh::data() {
	if(_vertices.size() != 0){

		ensureContext();

		_positionVbo.reset();
		_colorVbo.reset();
		_normalVbo.reset();
		_uvVbo.reset();
		_xMinMax = _yMinMax = _zMinMax = Vector2f(0.f);

		std::vector<GLfloat> pos, col, uv, norm;
		for(size_t i = 0; i < _vertices.size(); ++i){

			//Save minimums and maximums for bounding box
			_xMinMax.x = std::min(_xMinMax.x, _vertices[i].getPosition().x);
			_xMinMax.y = std::max(_xMinMax.y, _vertices[i].getPosition().x);
			_yMinMax.x = std::min(_yMinMax.x, _vertices[i].getPosition().y);
			_yMinMax.y = std::max(_yMinMax.y, _vertices[i].getPosition().y);
			_zMinMax.x = std::min(_zMinMax.x, _vertices[i].getPosition().z);
			_zMinMax.y = std::max(_zMinMax.y, _vertices[i].getPosition().z);

			_positionVbo.addData(&(_vertices[i].getPosition()), sizeof(Vector3f));

			_colorVbo.addData(&(_vertices[i].getColor()), sizeof(Vector3f));

			_uvVbo.addData(&(_vertices[i].getUv()), sizeof(Vector2f));

			_normalVbo.addData(&(_vertices[i].getNormal()), sizeof(Vector3f));
		}

		_positionVbo.uploadDataToGpu();
		_colorVbo.uploadDataToGpu();
		_normalVbo.uploadDataToGpu();
		_uvVbo.uploadDataToGpu();

		_model.recalculateBoundingBox();

		return true;

	}
	return false;
}

bool Mesh::isUpdated() const {
	return (!_needUpdate);
}

const Vector2f& Mesh::getXMinMax() const {
	return _xMinMax;
}

const Vector2f& Mesh::getYMinMax() const {
	return _yMinMax;
}

const Vector2f& Mesh::getZMinMax() const {
	return _zMinMax;
}

} /* namespace burn */
