/*
 * Text.h
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "../Export.h"
#include "GuiNode.h"

#include "Font.h"
#include "../System/String.h"

namespace burn {

class BURNGINE_API Text : public GuiNode {
public:
	Text();
	~Text();

	void setFontSize(const Uint32& size);
	const Uint32& getFontSize() const;

	void setText(const String& text);
	const String& getText() const;

	void draw();

	void setFont(const Font& font);
	const Font& getFont() const;

private:
	String _text;
	Uint32 _fontSize;
	Font _font;
};

} /* namespace burn */
#endif /* TEXT_H_ */
