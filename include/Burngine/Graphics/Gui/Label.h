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

#ifndef LABEL_H_
#define LABEL_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Gui/Text.h>
#include <Burngine/System/Math.h>

namespace burn {

/**
 * @brief This is an extension of simple Text. It adds
 * an automatically sized background to the text.
 */
class BURNGINE_API Label : public Text {
public:
	/**
	 * @brief Default constructor initializing some
	 * values
	 */
	Label();

	/**
	 * @brief This renders the text and the label's background.
	 *
	 * @note This might be deprecated in future versions.
	 */
	virtual void draw();

	/**
	 * @brief This sets the background color of the label
	 *
	 * @see getBackgroundColor()
	 */
	void setBackgroundColor(const Vector4f& color);

	/**
	 * @brief This returns the background color of the label's
	 * background.
	 *
	 * @return The background color
	 *
	 * @see setBackgroundColor()
	 */
	const Vector4f& getBackgroundColor() const;

	/**
	 * @brief This sets the margin of the label similar
	 * to the behaviour of HTML margin.
	 *
	 * @param margin The amount of margin in pixels
	 *
	 * @see getMargin()
	 */
	void setMargin(const float& margin);

	/**
	 * @brief This returns the amount of margin in pixels
	 *
	 * @return Amount of margin in pixels
	 */
	const float& getMargin() const;

private:
	Vector4f _backgroundColor;
	float _margin;
};

} /* namespace burn */
#endif /* LABEL_H_ */
