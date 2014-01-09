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

#include <Burngine/Graphics/Gui/Text.h>
#include <Burngine/System/Reporter.h>
#include <Burngine/Graphics/Window/Window.h>
#include <Burngine/Graphics/General/Shader.h>
#include <Burngine/Graphics/General/OpenGlControl.h>
#include <Burngine/Graphics/General/OpenGL.h>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace burn {

Text::Text() :
_fontSize(32),
_color(Vector4f(1.f, 1.f, 1.f, 1.f)) {
}

void Text::setText(const String& text) {
	_text = text;
}

void Text::draw() {

	//Valid OpenGL context has to exist
	ensureContext();

	//Draw the string to screen
	drawString();

}

void Text::drawString() {

	//The font has to be loaded obviously
	if(!_font.isLoaded())
		return;

	Vector2f curPosition(_position.x, _position.y);
	for(size_t i = 0; i < _text.getSize(); ++i){

		if(_text[i] == '\n'){
			curPosition.x = _position.x;
			curPosition.y -= _font.getNextLineOffset();
			continue;
		}else if(_text[i] == ' '){
			curPosition.x += _font.getSpaceOffset();
			continue;
		}

		const Character& character = _font.getCharacter(_text[i], _fontSize);
		curPosition.x += character.getBearing().x;

		character.draw(curPosition, _color);

		curPosition.x += (character.getAdvance().x - character.getBearing().x);
	}

}
void Text::setFontSize(const Uint32& size) {
	_fontSize = size;
}

void Text::setFont(const Font& font) {
	_font = font;
}

void Text::setColor(const Vector4f& color) {
	_color = color;
}

const Vector4f& Text::getColor() const {
	return _color;
}

} /* namespace burn */
