/*
 * Text.h
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "../Export.h"
#include "Transformable.h"

#include "Font.h"
#include "../System/String.h"

namespace burn {

class BURNGINE_API Text {
public:
	Text();
	~Text();

	void setFontSize(const Uint32& size);

	void setText(const String& text);
	void draw();

	void setFont(const Font& font);

	void setPosition(const Vector2f& pos);

private:
	String _text;
	Uint32 _fontSize;
	Font _font;
	Vector2f _position;
};

} /* namespace burn */
#endif /* TEXT_H_ */
