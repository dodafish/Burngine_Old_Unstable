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

#ifndef VERTEX_H_
#define VERTEX_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

namespace burn {

/**
 * @brief Contains position, color and uv.
 * Everything what a vertex can describe.
 */
class BURNGINE_API Vertex {
public:
	/**
	 * @The default constructor.
	 * You can set values or let them getting
	 * default. (All to 0)
	 *
	 * @param position The vertex Position
	 * @param color The vertex Color
	 * @param uv The vertex UV
	 * @param normal The vertex Normal
	 */
	Vertex(const Vector3f& position = Vector3f(), const Vector3f& color = Vector3f(1.0f, 1.0f, 1.0f),
	const Vector2f& uv = Vector2f(), const Vector3f& normal = Vector3f(1.0f, 1.0f, 1.0f));

	/**
	 * @brief The default destructor
	 */
	~Vertex();

	/**
	 * @brief Sets the position of the vertex
	 *
	 * @param position The new position
	 *
	 * @see getPosition()
	 */
	void setPosition(const Vector3f& position);

	/**
	 * @brief Returns the current position of the vertex
	 *
	 * @return The current position
	 *
	 * @see setPosition()
	 */
	const Vector3f& getPosition() const;

	/**
	 * @brief Sets the color of the vertex
	 *
	 * @param color The new color
	 *
	 * @see getColor()
	 */
	void setColor(const Vector3f& color);

	/**
	 * @brief Return the current color of the vertex
	 *
	 * @return The current color
	 *
	 * @see setColor()
	 */
	const Vector3f& getColor() const;

	/**
	 * @brief Sets the current UV-coordinates of the vertex
	 *
	 * @param uv The new UV-coordinates
	 *
	 * @see getUv()
	 *
	 * @note UV-coordinates' range is from 0 to 1
	 */
	void setUv(const Vector2f& uv);

	/**
	 * @brief Return the current UV-coordinates of the vertex
	 *
	 * @return The current UV-coordinates
	 *
	 * @see setUv()
	 */
	const Vector2f& getUv() const;

	/**
	 * @brief Sets the current normal-coordinates of the vertex
	 *
	 * @param normal The new normal-coordinates
	 *
	 * @see getNormal()
	 */
	void setNormal(const Vector3f& normal);

	/**
	 * @brief Return the current normal-coordinates of the vertex
	 *
	 * @return The current normal-coordinates
	 *
	 * @see setNormal()
	 */
	const Vector3f& getNormal() const;

private:
	Vector3f _position, _color, _normal;
	Vector2f _uv;
};

} /* namespace burn */
#endif /* VERTEX_H_ */
