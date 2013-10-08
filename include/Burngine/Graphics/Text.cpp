/*
 * Text.cpp
 *
 *  Created on: 06.10.2013
 *      Author: Dominik
 */

#include "Text.h"
#include "../System/Reporter.h"

namespace burn {

Text::Text() :
_fontSize(32) {
}

Text::~Text() {
}

void Text::setText(const String& text) {
	_text = text;
}

void Text::draw() {

	if(!_font.isLoaded())
		return;

	for(size_t i = 0; i < _text.size(); ++i)
		_font.getCharacter(_text[i]).draw(Vector2i(_position.x, _position.y));

}

void Text::setFontSize(const Uint32& size) {
	_fontSize = size;
}

void Text::setFont(const Font& font) {
	_font = font;
}

} /* namespace burn */
