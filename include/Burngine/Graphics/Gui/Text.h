/*
 * Text.h
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

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
