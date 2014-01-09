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

#ifndef TEXT_H_
#define TEXT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Gui/GuiNode.h>
#include <Burngine/Graphics/Gui/Font.h>
#include <Burngine/System/String.h>

namespace burn {

class BURNGINE_API Text : public GuiNode {
public:
	/**
	 * @brief Default constructor initializing some
	 * values
	 */
	Text();

	/**
	 * @brief Sets the Text to a fontsize.
	 *
	 * @param size The size in pixels the Text shall be
	 *
	 * @see getFontSize()
	 */
	void setFontSize(const Uint32& size);

	/**
	 * @brief Returns the fontsize of the Text
	 *
	 * @return The size in pixels
	 *
	 * @see setFontSize()
	 */
	const Uint32& getFontSize() const;

	/**
	 * @brief Sets the content of the Text.
	 *
	 * @param text The string that shall be displayed
	 *
	 * @see getText()
	 */
	void setText(const String& text);

	/**
	 * @brief Returns the content of the Text
	 *
	 * @return The content of the Text as a string
	 *
	 * @see setText()
	 */
	const String& getText() const;

	/**
	 * @brief This draws the Text. A loaded font
	 * has to be set and a content too.
	 *
	 * @note This might be deprecated in future versions
	 *
	 * @see setText()
	 * @see setFont()
	 */
	virtual void draw();

	/**
	 * @brief This sets a Font to the Text with which
	 * the Text will be drawn.
	 *
	 * @param font The Font to use
	 *
	 * @see getFont()
	 */
	void setFont(const Font& font);

	/**
	 * @brief Returns the Font that has been set to the
	 * Text.
	 *
	 * @return The set Font
	 *
	 * @see setFont()
	 */
	const Font& getFont() const;

	/**
	 * @brief This sets the color of the Text with which
	 * it will be drawn.
	 *
	 * @param color The color that the Text shall have
	 *
	 * @see getColor()
	 */
	void setColor(const Vector4f& color);

	/**
	 * @brief This returns the color the
	 * Text is being drawn with.
	 *
	 * @return The color of the Text
	 *
	 * @see setColor()
	 */
	const Vector4f& getColor() const;

protected:
	/**
	 * @brief Extra function for drawing the text.
	 * Used by other classes like Label that inherit from
	 * Text
	 */
	void drawString();

	String _text;
	Font _font;
	Uint32 _fontSize;

private:
	Vector4f _color;
};

} /* namespace burn */
#endif /* TEXT_H_ */
