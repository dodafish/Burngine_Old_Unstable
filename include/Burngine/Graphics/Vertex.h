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

class BURNGINE_API Vertex {
public:
	Vertex();
	~Vertex();

	void setPosition(const Vector3f& position);
	const Vector3f& getPosition() const;

	void setColor(const Vector3f& color);
	const Vector3f& getColor() const;

	void setUv(const Vector2f& uv);
	const Vector2f& getUv() const;

private:
	Vector3f _position, _color;
	Vector2f _uv;
};

} /* namespace burn */
#endif /* VERTEX_H_ */
