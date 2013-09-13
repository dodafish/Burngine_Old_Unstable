/*
 * Vertex.cpp
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#include "Vertex.h"

namespace burn {

Vertex::Vertex(const Vector3f& position, const Vector3f& color,
		const Vector2f& uv) :
				_position(position),
				_color(color),
				_uv(uv)

{

}

Vertex::~Vertex() {
	// TODO Auto-generated destructor stub
}

void Vertex::setPosition(const Vector3f& position) {
	_position = position;
}

const Vector3f& Vertex::getPosition() const {
	return _position;
}

void Vertex::setColor(const Vector3f& color) {
	_color = color;
}

const Vector3f& Vertex::getColor() const {
	return _color;
}

void Vertex::setUv(const Vector2f& uv) {
	_uv = uv;
}

const Vector2f& Vertex::getUv() const {
	return _uv;
}

} /* namespace burn */
