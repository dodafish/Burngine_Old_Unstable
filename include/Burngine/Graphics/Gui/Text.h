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

#ifndef TEXT_H_
#define TEXT_H_

#include <Burngine/Export.h>
#include <Burngine/Graphics/Gui/GuiNode.h>
#include <Burngine/Graphics/Gui/Font.h>
#include <Burngine/System/String.h>

namespace burn {

class BURNGINE_API Text : public GuiNode {
public:
	Text();
	~Text();

	void setFontSize(const Uint32& size);
	const Uint32& getFontSize() const;

	void setText(const String& text);
	const String& getText() const;

	virtual void draw();

	void setFont(const Font& font);
	const Font& getFont() const;

	void setColor(const Vector4f& color);
	const Vector4f& getColor() const;

protected:
	/**
	 * @brief Extra function for drawing the text.
	 * Used by other classes like Label, that inherit from
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
