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

#include <Burngine/Graphics/General/Vertex.h>

namespace burn {

Vertex::Vertex(	const Vector3f& position,
				const Vector2f& uv,
				const Vector3f& normal,
				const Vector3f& tangent,
				const Vector3f& bitangent) :
_position(position),
_normal(normal),
_tangent(tangent),
_bitangent(bitangent),
_uv(uv) {
}

Vertex::~Vertex() {
}

void Vertex::setPosition(const Vector3f& position) {
	_position = position;
}

const Vector3f& Vertex::getPosition() const {
	return _position;
}

void Vertex::setNormal(const Vector3f& normal) {
	_normal = normal;
}

const Vector3f& Vertex::getNormal() const {
	return _normal;
}

void Vertex::setTangent(const Vector3f& tangent) {
	_tangent = tangent;
}

const Vector3f& Vertex::getTangent() const {
	return _tangent;
}

void Vertex::setBitangent(const Vector3f& bitangent) {
	_bitangent = bitangent;
}

const Vector3f& Vertex::getBitangent() const {
	return _bitangent;
}

void Vertex::setUv(const Vector2f& uv) {
	_uv = uv;
}

const Vector2f& Vertex::getUv() const {
	return _uv;
}

} /* namespace burn */
