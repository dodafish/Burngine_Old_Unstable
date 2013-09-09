/*
 * Vertex.h
 *
 *  Created on: 09.09.2013
 *      Author: Dominik
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "../Export.h"
#include "../System/Math.h"

namespace burn {

class Vertex {
public:
	Vertex();
	~Vertex();
private:
	Vector3f _position, _color;
	Vector2f _uv;
};

} /* namespace burn */
#endif /* VERTEX_H_ */
