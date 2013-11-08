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

#ifndef RECTANGLESHAPE_H_
#define RECTANGLESHAPE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>
#include <Burngine/Graphics/Gui/GuiNode.h>

namespace burn {

class BURNGINE_API RectangleShape : public GuiNode {
public:
	/**
	 * @brief Default constructor
	 */
	RectangleShape();

	/**
	 * @brief Sets the dimensions of the rectangle
	 *
	 * @param dimensions Width and height of the rectangle
	 *
	 * @see getDimensions()
	 */
	void setDimensions(const Vector2f& dimensions);

	/**
	 * @brief Returns the dimensions of the rectangle in
	 * width and height
	 *
	 * @return The dimensions of the rectangle
	 *
	 * @see setDimensions()
	 */
	const Vector2f& getDimensions() const;

	/**
	 * @brief Sets the color of the rectangle
	 *
	 * @param color 4-Component color in range [0, 1]
	 *
	 * @see getColor()
	 */
	void setColor(const Vector4f& color);

	/**
	 * @brief Returns a 4-component color in range
	 * [0, 1]
	 *
	 * @return A 4-component color in range [0, 1]
	 *
	 * @see setColor()
	 */
	const Vector4f& getColor() const;

	/**
	 * @brief Draws the rectangle to the screen
	 *
	 * @note Might be depracated in future versions
	 */
	void draw();

private:
	Vector2f _dimensions;
	Vector4f _color;
};

} /* namespace burn */
#endif /* RECTANGLESHAPE_H_ */
