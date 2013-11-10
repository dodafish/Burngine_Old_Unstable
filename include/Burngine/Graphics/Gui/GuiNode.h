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

#ifndef GUINODE_H_
#define GUINODE_H_

#include <Burngine/Export.h>
#include <Burngine/System/Math.h>

#include <vector>

namespace burn {
class Gui;
}

template class BURNGINE_API std::vector<burn::Gui*>;

namespace burn {

/**
 * @brief Baseclass for all GuiNodes providing some transform features
 */
class BURNGINE_API GuiNode {
public:
	/**
	 * @brief Default constructor initializing some
	 * values
	 */
	GuiNode();

	/**
	 * @brief Default destructor cleaning up GuiNode and
	 * automatically detaching it from Guis
	 */
	virtual ~GuiNode();

	/**
	 * @brief Virtual method for rendering calls.
	 *
	 * @note This might be deprecated in future versions
	 */
	virtual void draw() = 0;

	/**
	 * @brief Sets the position of the GuiNode in pixel
	 * coordinates
	 *
	 * @param position Position in pixel coordinates
	 *
	 * @note The origin is the lower left corner
	 *
	 * @see getPosition()
	 */
	void setPosition(const Vector2f& position);

	/**
	 * @brief Returns the current position of the GuiNode
	 * in pixel coordinates.
	 *
	 * @return The current position in pixel coordinates
	 *
	 * @note The origin is the lower left corner
	 *
	 * @see setPosition()
	 */
	const Vector2f& getPosition() const;

	/**
	 * @brief Sets the rotation of the GuiNode.
	 *
	 * @param rotation Rotation in angles
	 *
	 * @note Initial value is 0 degrees. Positive
	 * degrees rotate clockwise.
	 */
	void setRotation(const float& rotation);

	/**
	 * @brief Returns the current rotation of the GuiNode.
	 *
	 * @return Current rotation
	 *
	 * @note Initial value is 0 degrees. Positive
	 * degrees rotate clockwise.
	 */
	const float& getRotation() const;

	/**
	 * @brief Changes rotation by the given offset
	 *
	 * @param angle The offset in degrees
	 *
	 * @note Positive degrees rotate clockwise
	 */
	void rotate(const float& angle);

	/**
	 * @brief Moves the GuiNode by the given offset
	 *
	 * @param offset Offset in pixels
	 *
	 * @note The origin is the lower left corner.
	 */
	void move(const Vector2f& offset);

	/**
	 * @brief Moves the GuiNode by the given offset
	 *
	 * @param offset Offset in pixels
	 *
	 * @note The origin is the lower left corner.
	 */
	void move(const float& offsetX, const float& offsetY);

	/**
	 * @brief Sets the Z-Index of the GuiNode. The higher the
	 * Z-Index is the more important is the GuiNode and it will
	 * be drawn on top of lower Z-Indices.
	 *
	 * @param zIndex The Z-Index the GuiNode shall get
	 *
	 * @see getZIndex()
	 */
	void setZIndex(const Int32& zIndex);

	/**
	 * @brief Returns the current Z-Index of the GuiNode
	 *
	 * @return Current Z-Index of the GuiNode
	 *
	 * @see setZIndex()
	 */
	const Int32& getZIndex() const;

	/**
	 * @brief Adds a Gui as parent. This is used by Gui when
	 * attaching GuiNodes. You should use the Gui's methods for
	 * connecting GuiNodes to Guis instead of this method
	 *
	 * @param parent The parent Gui
	 *
	 * @see removeParentGui()
	 */
	void addParentGui(Gui* parent);

	/**
	 * @brief Removes a Gui from parents. This is used by Gui when
	 * detaching GuiNodes. You should use the Gui's methods for
	 * disconnecting GuiNodes from Guis instead of this method
	 *
	 * @param parent The parent Gui
	 *
	 * @see addParentGui()
	 */
	void removeParentGui(Gui* parent);

protected:
	Vector2f _position;
	float _rotation;
	Int32 _zIndex;
	std::vector<Gui*> _parents;
};

} /* namespace burn */
#endif /* GUINODE_H_ */
